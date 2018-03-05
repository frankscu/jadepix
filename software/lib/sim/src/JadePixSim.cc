#include "JadePixSim.hh"

JadePixSim::JadePixSim():m_macros("test2_Fe55.mac"),m_file("JadePix.rawdat")
{
}

JadePixSim::~JadePixSim()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void JadePixSim::runSim(G4int &argc, char** &argv)
{
    // Choose the Random engine
    //
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    G4int seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);

    // Construct the default run manager
    //
    G4RunManager* runManager = new G4RunManager;

    // Set mandatory initialization classes
    //
    JadePixDetectorConstruction* detConstruction = new JadePixDetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    JadePixPhysicsList* physicsList = new JadePixPhysicsList();
    runManager->SetUserInitialization(physicsList);

    // Set user action classes
    //
    SourceType st = _GeneralParticleSource;
    runManager->SetUserAction(new JadePixPrimaryGeneratorAction(st));
    //
    JadePixRunAction* jRun = new JadePixRunAction();

    if(argc > 2){
        m_file = argv[2];
    }

    jRun->SetOutFileName(m_file);

    runManager->SetUserAction(jRun);

    runManager->SetUserAction(new JadePixEventAction());

    // Initialize G4 kernel
    //
    runManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    JadePixMySession* LoggedSession = new JadePixMySession();
    UImanager->SetCoutDestination(LoggedSession);
    LoggedSession->SessionStart();

    UImanager->ApplyCommand("/control/macroPath /home/chenlj/jadepix/macros/geant4");
    
    if(argc != 1){
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }else{

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

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;
    delete LoggedSession;    
}

