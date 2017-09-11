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
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"
#include "B1Hit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"

#include "G4ios.hh"

#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction()
: G4UserEventAction()
{
 fHCID=-1;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event* evt)
{ 
    G4int evtNb = evt->GetEventID();
    if(evtNb%1000==0)
        G4cout << "\n ------------> Begin of Event: " << evtNb << G4endl;

    G4SDManager* SDManager = G4SDManager::GetSDMpointer();
    if ( fHCID==-1 )
        fHCID = SDManager->GetCollectionID("HitsCollection"); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event)
{   	
    // accumulate statistics in B1Run
    eventID = event->GetEventID();

    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    B1HitsCollection* SH
        = static_cast<B1HitsCollection*>(HCE->GetHC(fHCID));
    if(SH) SH->DrawAllHits();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4int n_hit = SH->entries();
    G4double fEdep = 0;

    analysisManager->FillH1(0, n_hit);

    for (G4int i=0; i<n_hit; i++)
    {
        B1Hit* hit = (*SH)[i];

        //Position
        G4ThreeVector fPos = hit->GetPos();
        analysisManager->FillH2(0, fPos.x(), fPos.z());

        //Energy
        G4double eDep = hit->GetEdep();
        if (eDep > 0.) 
        {
            fEdep += eDep ;	
        } 

        std::fstream fout1("B1step.txt", std::ios::out | std::ios::app);
        fout1  << eventID << "      "
            << hit->GetShapeNb() << "    " 
            << eDep << "     "
            << fPos.x() << "     "
            << fPos.y() << "     "
            << fPos.z() << "     "
            << "\n" << G4endl;
        analysisManager->FillNtupleDColumn(1,0, eventID);
        analysisManager->FillNtupleDColumn(1,1, hit->GetShapeNb());
        analysisManager->FillNtupleDColumn(1,2, eDep);
        analysisManager->FillNtupleDColumn(1,3, fPos.x());
        analysisManager->FillNtupleDColumn(1,4, fPos.y());
        analysisManager->FillNtupleDColumn(1,5, fPos.z());
        analysisManager->AddNtupleRow(1); 
        fout1.close();

    }

    analysisManager->FillH1(1, fEdep);
    analysisManager->FillNtupleDColumn(0,0, fEdep);
    analysisManager->AddNtupleRow(0); 

    std::fstream fout("B1data.txt", std::ios::out | std::ios::app);
    fout  << fEdep  
        << "\n" << G4endl;
    fout.close();


    B1Run* run 
        = static_cast<B1Run*>(
                G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    run->AddEdep(fEdep); 

    const G4double eThreshold = 400.*eV;
    if (fEdep > eThreshold) run->AddfGoodEvents();


    // get number of stored trajectories
    G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
    G4int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

    // periodic printing
    //G4int eventID = event->GetEventID();
    //if ( eventID < 100 || eventID % 100 == 0) 
    //{
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) 
    {
        G4cout << "    " << n_trajectories
            << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "  
        << hc->GetSize() << " hits stored in this event" << G4endl;
    //}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
