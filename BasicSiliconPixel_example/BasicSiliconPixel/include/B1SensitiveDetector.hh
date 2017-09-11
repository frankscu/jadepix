#ifndef B1CaloSensitiveDetector_h
#define B1SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "B1Hit.hh"
#include "B1EventAction.hh"
#include <vector>




class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
class B1SensitiveDetector : public G4VSensitiveDetector
{
public:
  /// Constructor
  B1SensitiveDetector(G4String SDname);
  /// Destructor
  ~B1SensitiveDetector();

  // methods from base class
  void Initialize(G4HCofThisEvent* HCE);
  
  G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  
  void EndOfEvent(G4HCofThisEvent* HCE);


private:
  B1HitsCollection* fHitsCollection;
  G4String sensitiveDetectorName;
  G4String hitsCollectionName;
};

#endif
