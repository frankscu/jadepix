#include "JadePixSim.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
JadePixSim::JadePixSim(int argc, char** argv)
{
    // Choose the Random engine
    //
    //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4int seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);

    // Construct the default run manager
    //
    runManager = new G4RunManager;

    // Set mandatory initialization classes
    //
    JadePixDetectorConstruction* detConstruction = new JadePixDetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    G4VModularPhysicsList* physicsList = new QGSP_BERT;
    //G4VModularPhysicsList* physicsList = new FTFP_BERT;
    //  JadePixPhysicsList* physicsList = new JadePixPhysicsList;
    runManager->SetUserInitialization(physicsList);

    // Set user action classes
    //
    SourceType st = _GeneralParticleSource;
    runManager->SetUserAction(new JadePixPrimaryGeneratorAction(st));
    //
    JadePixRunAction* jRun = new JadePixRunAction();
    if(argc >= 2){
        if( strcmp(argv[2],"-b")!=0){
        jRun->SetOutFileName(argv[2]);
        }else{
            jRun->SetOutFileName(argv[3]);
        }
    }
    runManager->SetUserAction(jRun);
    //
    runManager->SetUserAction(new JadePixEventAction());

    // Initialize G4 kernel
    //
    runManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    //  G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    LoggedSession = new JadePixMySession();
    UImanager->SetCoutDestination(LoggedSession);
    LoggedSession->SessionStart();

    UImanager->ApplyCommand("/control/macroPath /home/chenlj/jadepix/macros/geant4");

    if (argc != 1)   // batch mode
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
        UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
#endif
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
//
JadePixSim::~JadePixSim(){;
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;
    delete LoggedSession;
}
