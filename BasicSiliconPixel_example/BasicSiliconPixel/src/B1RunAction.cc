//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1RunAction.cc 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction()
{         
   // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000); 

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("B1"); 

  analysisManager
	 ->CreateH1("Shape1", "Drift Shape1 # Hits", 1000, 0., 100);//h1 Id=0 
  analysisManager
	 ->CreateH1("Shape1 Enegry", "Edep in Shape1", 1000, 0., 100.*keV);//h1 Id=1

  analysisManager
	  ->CreateH2("Shape XZ", "Drift Shape1 X vs Z", 
			  1000, -4., 4.*mm, 1000, -4., 4.*mm ); //h2 Id=0
  analysisManager
	  ->CreateH2("Shape_0 XZ", "Drift Shape X vs Z", 
			  1000, -4., 4.*mm, 1000, -4., 4.*mm ); //h2 Id=1
  
  analysisManager->CreateNtuple("B1","Events");
  analysisManager->CreateNtupleDColumn("Shape1Energy"); //column Id=0

  analysisManager->CreateNtuple("B1Hits","Hits");
  analysisManager->CreateNtupleDColumn("eventID");
  analysisManager->CreateNtupleDColumn("ShapeNb");
  analysisManager->CreateNtupleDColumn("HitsEdep");
  analysisManager->CreateNtupleDColumn("Xposition");
  analysisManager->CreateNtupleDColumn("Yposition"); 
  analysisManager->CreateNtupleDColumn("Zposition");
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
 delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B1RunAction::GenerateRun()
{
  return new B1Run; 
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* aRun)
{
  
  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const B1Run* b1Run = static_cast<const B1Run*>(aRun);  
  G4double edep  = b1Run->GetEdep();
  G4double edep2 = b1Run->GetEdep2();
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;
  
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  


  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    G4GeneralParticleSource* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  } 

  
  G4int nbGoodEvents = b1Run->GetNbGoodEvents();
  

  std::fstream fout("B1GoodEvents.txt", std::ios::out | std::ios::app);
  fout << G4BestUnit(particleEnergy,"Energy") 
       << "      " 
       << nbGoodEvents << "\n"
       << G4endl;	

  // Print
  //  
  if (IsMaster()) {
    G4cout
     << "\n--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << "\n--------------------End of Local Run------------------------";
  }

  G4cout
     << "\n The run consists of " << nofEvents << " "<< runCondition
     << "\n------------------------------------------------------------\n"
     << " The number of good events: " << nbGoodEvents << "\n" 
     << " The energy deposit per Run is "
     << G4BestUnit(edep,"Energy")<<" +- "<< G4BestUnit(rms,"Energy")
     << "\n------------------------------------------------------------\n"
     << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
