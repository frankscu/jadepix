#ifndef JADEPIXSIM_H 
#define JADEPIXSIM_H 

#include "JadePixDetectorConstruction.hh"
#include "JadePixPrimaryGeneratorAction.hh"
#include "JadePixRunAction.hh"
#include "JadePixEventAction.hh"

#include "JadePixMySession.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT.hh"

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

class JadePixSim{
    public:
        JadePixSim(int argc, char** argv);
        ~JadePixSim();
    private:
        G4VisManager* visManager;
        G4RunManager* runManager;
        JadePixMySession* LoggedSession;
};

#endif //JADEPIXSIM_H
