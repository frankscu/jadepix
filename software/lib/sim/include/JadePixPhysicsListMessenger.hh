#ifndef JadePixPhysicsListMessenger_h
#define JadePixPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class JadePixPhysicsList;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JadePixPhysicsListMessenger: public G4UImessenger
{
public:
  
  JadePixPhysicsListMessenger(JadePixPhysicsList* p = 0);
  virtual ~JadePixPhysicsListMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  JadePixPhysicsList* pJadePixPhysicsList;
    
  G4UIcmdWithADoubleAndUnit* gammaCutCmd;
  G4UIcmdWithADoubleAndUnit* electCutCmd;
  G4UIcmdWithADoubleAndUnit* posCutCmd;
  G4UIcmdWithADoubleAndUnit* pCutCmd;
  G4UIcmdWithADoubleAndUnit* allCutCmd;
  G4UIcmdWithAString*        pListCmd;
  G4UIcmdWithoutParameter*   listCmd;  
  G4UIcmdWithAnInteger* verboseCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

