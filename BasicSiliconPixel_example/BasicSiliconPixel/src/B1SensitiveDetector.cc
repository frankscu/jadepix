#include "B1SensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "B1EventAction.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"



B1SensitiveDetector::B1SensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  G4cout<<"*************************************" << G4endl;
  G4cout<<"*** Creating SD with name: "<<SDname<< " ***" << G4endl;  
  G4cout<<"*************************************" << G4endl;
  sensitiveDetectorName = SDname;

  G4String HCname;
  collectionName.insert(HCname="HitsCollection");
  fHitsCollection=NULL;

}

B1SensitiveDetector::~B1SensitiveDetector()
{}

void B1SensitiveDetector::Initialize(G4HCofThisEvent*)
{
  G4cout << "collectionName" << collectionName[0]<<G4endl;
  fHitsCollection = new B1HitsCollection(sensitiveDetectorName, collectionName[0]);
  
}

G4bool B1SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory* )
{
  
  B1Hit* detectorhit = new B1Hit();
  G4double edep = aStep->GetTotalEnergyDeposit();

  detectorhit->SetTrackID(aStep->GetTrack()->GetTrackID());
  detectorhit->SetShapeNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
  detectorhit->SetEdep(edep);
  detectorhit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  detectorhit->SetPolarization(aStep->GetPostStepPoint()->GetPolarization());

  fHitsCollection->insert(detectorhit);
  return true;
}

void B1SensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
    static G4int HCID= -1;
    if(HCID < 0)
    {
        HCID=GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID,fHitsCollection);

    if(fHitsCollection)
    {
        G4int NumOfHits = fHitsCollection->entries();
        for (G4int i=0; i<NumOfHits; i++ )
        {
            B1Hit* hit = (*fHitsCollection)[i];
            hit->Print();	
        }
    }

}
