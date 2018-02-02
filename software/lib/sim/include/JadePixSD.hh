#ifndef JadePixAnalysis_h
#define JadePixAnalysis_h 1

#include "JadePixSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "JadePixHit.hh"
#include "JadePixGeoParameter.hh"
#include "JadePixIdentifier.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
class JadePixGeoParameter;
class JadePixIdentifier;

class JadePixSD : public G4VSensitiveDetector
{
    public:

        JadePixSD(const G4String&);
        ~JadePixSD();

        void Initialize(G4HCofThisEvent* HCE);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent* );

    private:
        JadePixHitsCollection* hitsCollection;
        JadePixHitsCollection* truthCollection;
        JadePixGeoParameter* JadePixGeoPointer;
        G4String SensitiveDetectorName;
        std::map<G4int, G4int> hitMap; //FIX ME
        std::map<G4int, G4int> truthMap; //FIX ME
        std::map<G4int, G4int>::iterator itTruthMap; //FIX ME

};

#endif
