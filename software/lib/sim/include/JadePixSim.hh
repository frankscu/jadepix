#ifndef JADEPIXSIM_H 
#define JADEPIXSIM_H 

#include "JadePixDetectorConstruction.hh"
#include "JadePixPrimaryGeneratorAction.hh"
#include "JadePixRunAction.hh"
#include "JadePixEventAction.hh"
#include "JadePixMySession.hh"
#include "JadePixPhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

class G4VisManager;
class G4RunManager;
class JadePixMySession;
class JadePixPhysicsList;

class JadePixSim{
    public:
        JadePixSim();
        virtual ~JadePixSim();
        void runSim(G4int &argc, char** &argv);

    private:
        std::string m_macros;
        std::string m_file;
};

#endif //JADEPIXSIM_H
