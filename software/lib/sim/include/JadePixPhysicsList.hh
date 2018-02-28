#ifndef JadePixPhysicsList_h
#define JadePixPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class JadePixPhysicsListMessenger;

class JadePixPhysicsList: public G4VModularPhysicsList
{
public:
    JadePixPhysicsList();
    virtual ~JadePixPhysicsList();
    
    void ConstructParticle();
    void SetVerbose(G4int verbose);
    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
    void SetCutForProton(G4double);
   
    void AddJadePixPhysicsList(const G4String& name); 
    void ConstructProcess();
    void AddStepMax();
    void List();

private:

    void SetBuilderList0(G4bool flagHP = false);
    void SetBuilderList1(G4bool flagHP = false);
    void SetBuilderList2(G4bool addStopping = false);
    void SetBuilderList3();
    void SetBuilderList4();

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double cutForProton;

    G4VPhysicsConstructor* emJadePixPhysicsList;
    G4VPhysicsConstructor* particleList;
    std::vector<G4VPhysicsConstructor*> hadronPhys;

    JadePixPhysicsListMessenger* m_Messenger;
    G4bool dump;
};

#endif

 
