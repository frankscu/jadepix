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
// $Id$
//
/// \file JadePixEventAction.cc
/// \brief Implementation of the JadePixEventAction class

#include "JadePixEventAction.hh"
#include "JadePixEventActionMessenger.hh"
#include "JadePixSD.hh"
#include "JadePixHit.hh"
#include "JadePixAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "JadePixDigi.hh"
#include "JadePixDigitizer.hh"
#include "G4DigiManager.hh"

#include "JadePixWriter.hh"

#include "Randomize.hh"
#include "G4ios.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixEventAction::JadePixEventAction()
  : G4UserEventAction(),
  fMessenger(0),
  fPrintModulo(1)
{
  fMessenger = new JadePixEventActionMessenger(this);
  G4DigiManager * fDM = G4DigiManager::GetDMpointer();
  JadePixDigitizer * JadePixDTZ = new JadePixDigitizer( "JadePixDigitizer" );
  fDM->AddNewModule(JadePixDTZ);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixEventAction::~JadePixEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixHitsCollection*
JadePixEventAction::GetHitsCollection(const G4String& hcName,
    const G4Event* event) const
{
  G4int HCID
    = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);

  JadePixHitsCollection* hitsCollection
    = static_cast<JadePixHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(HCID));


  if ( ! hitsCollection ) {
    G4cerr << "JadePixEventAction :: Cannot access hitsCollection " << hcName << G4endl;
    exit(1);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixEventAction::PrintEventStatistics(G4double hitEdep, G4double truthEdep) const
{
  // print event statistics
  G4cout
    << "   Hit: total energy: "
    << std::setw(7) << G4BestUnit(hitEdep, "Energy")
    << G4endl
    << "   Truth: total energy: "
    << std::setw(7) << G4BestUnit(truthEdep, "Energy")
    << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixEventAction::BeginOfEventAction(const G4Event* event)
{

  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0 )  {
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixEventAction::EndOfEventAction(const G4Event* event)
{
  //Digitize
  G4DigiManager * fDM = G4DigiManager::GetDMpointer();
  JadePixDigitizer * JadePixDTZ = (JadePixDigitizer*)fDM->FindDigitizerModule( "JadePixDigitizer" );
  JadePixDTZ->Digitize();
  G4int myDigiCollID = fDM->GetDigiCollectionID("JadePixDigisCollection");
  JadePixDigisCollection* digiC = (JadePixDigisCollection*)fDM->GetDigiCollection( myDigiCollID );

  // Get hits collections
  JadePixHitsCollection* truthC
    = GetHitsCollection("JadePixTruthCollection", event);

  if(truthC->entries()==0){
    if ( event->GetEventID() % fPrintModulo == 0 )  {
      G4cerr << "JadePixEventAction :: There is no hits in JadePixHitsCollections!!! " << G4endl;
    }
      //exit(1);
    return;
  }

  JadePixWriter* mWriter = JadePixWriter::Instance();

  mWriter->WriteEventTag(event->GetEventID());

  //Write McTruth
  //G4int nofMc=truthC->entries();
  //mWriter->WriteMcTag(nofMc);
  //for(G4int iMc=0;iMc<nofMc;iMc++){
  //  JadePixHit* truthHit = (*truthC)[iMc];
  //  mWriter->WriteMc(truthHit);
  //}

  G4double eT = 0;
  G4int nofDigi = digiC->entries();
  //mWriter->WriteDigiTag(nofDigi);
  for(G4int iDigi=0;iDigi<nofDigi;iDigi++){
    JadePixDigi* digiHit = (*digiC)[iDigi];
    //mWriter->WriteDigi(digiHit);
    eT += digiHit->GetEdep();
  }
  //std::cout<<" Edep: "<<G4BestUnit(eT,"Energy")<<endl;

  //Write McTruth
  G4int nofMc=truthC->entries();
  mWriter->WriteMcTag(nofMc);
  for(G4int iMc=0;iMc<nofMc;iMc++){
    JadePixHit* truthHit = (*truthC)[iMc];
    mWriter->WriteMcDebug(truthHit, eT);
  }

  mWriter->WriteDigiTag(nofDigi);
  for(G4int iDigi=0;iDigi<nofDigi;iDigi++){
    JadePixDigi* digiHit = (*digiC)[iDigi];
    mWriter->WriteDigi(digiHit);
    //eT += digiHit->GetEdep();
  }

  // Print per event (modulo n)
  G4int eventID = event->GetEventID();
  if ( eventID % fPrintModulo == 0) {
    G4cout << "---> End of event: " << eventID << G4endl;
    //      PrintEventStatistics(newHit->GetEdep(),truthHit->GetEdep());
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

