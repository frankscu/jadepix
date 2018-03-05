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
//
// $Id: PhysicsList.cc,v 1.37 2010-11-19 20:12:32 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
/////////////////////////////////////////////////////////////////////////
//
// PhysicsList
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
// 26.04.2007 Physics according to 8.3 Physics List (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#include "JadePixPhysicsList.hh"
#include "JadePixPhysicsListMessenger.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"

#include "G4HadronInelasticQBBC.hh"

//#include "G4EmCompPhotoEPhysics.hh"

#include "G4IonPhysics.hh"

#include "G4LossTableManager.hh"
#include "G4StepLimiter.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

JadePixPhysicsList::JadePixPhysicsList() : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue = 0.010*mm;
    cutForGamma     = defaultCutValue;
    cutForElectron  = defaultCutValue;
    cutForPositron  = defaultCutValue;
    cutForProton    = defaultCutValue;
    verboseLevel    = 1;

    m_Messenger = new JadePixPhysicsListMessenger(this);

    // Particles
    particleList = new G4DecayPhysics("decays");

    // EM physics
    emJadePixPhysicsList = new G4EmStandardPhysics();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

JadePixPhysicsList::~JadePixPhysicsList()
{
    delete m_Messenger;
    delete particleList;
    delete emJadePixPhysicsList;
    for(size_t i=0; i<hadronPhys.size(); i++) {
        delete hadronPhys[i];
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::ConstructParticle()
{
    particleList->ConstructParticle();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::ConstructProcess()
{
    AddTransportation();
    emJadePixPhysicsList->ConstructProcess();
    particleList->ConstructProcess();
    for(size_t i=0; i<hadronPhys.size(); i++) {
        hadronPhys[i]->ConstructProcess();
    }
    AddStepMax();
}

void JadePixPhysicsList::SetVerbose(G4int verbose)
{
    emJadePixPhysicsList->SetVerboseLevel(verbose);
    for(size_t i=0; i<hadronPhys.size(); i++) {
        hadronPhys[i]->SetVerboseLevel(verbose);
    }
}

void JadePixPhysicsList::AddStepMax()
{

    // Step limitation seen as a process
    G4StepLimiter* stepLimiter = new G4StepLimiter();

    auto particleIterator = GetParticleIterator();
    particleIterator->reset();

    while ((*particleIterator)()){

        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        if (particle->GetPDGCharge() != 0.0)
        {
            pmanager->AddDiscreteProcess(stepLimiter);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::AddJadePixPhysicsList(const G4String& name)
{
    if (verboseLevel>0) {
        G4cout << "JadePixPhysicsList::AddJadePixPhysicsList: <" << name << ">" << G4endl;
    }
    if (name == "emstandard_opt2") {

        delete emJadePixPhysicsList;
        emJadePixPhysicsList = new G4EmStandardPhysics_option2();

    } else if (name == "emstandard_opt3") {

        delete emJadePixPhysicsList;
        emJadePixPhysicsList = new G4EmStandardPhysics_option3();

    } else if (name == "emstandard_opt1") {

        delete emJadePixPhysicsList;
        emJadePixPhysicsList = new G4EmStandardPhysics_option1();

    } else if (name == "emstandard_opt0") {

        delete emJadePixPhysicsList;
        emJadePixPhysicsList = new G4EmStandardPhysics();

    } else if (name == "FTFP_BERT_EMV") {

        AddJadePixPhysicsList("emstandard_opt1");
        AddJadePixPhysicsList("FTFP_BERT");

    }else if (name == "FTFP_BERT_EMY") {

        AddJadePixPhysicsList("emstandard_opt3");
        AddJadePixPhysicsList("FTFP_BERT");

    } else if (name == "FTFP_BERT_EMX") {

        AddJadePixPhysicsList("emstandard_opt2");
        AddJadePixPhysicsList("FTFP_BERT");

    }

    else if (name == "QGSP_BERT_EMV") {

        AddJadePixPhysicsList("emstandard_opt1");
        AddJadePixPhysicsList("QGSP_BERT");

    } else if (name == "QGSP_BERT_EMX") {

        AddJadePixPhysicsList("emstandard_opt2");
        AddJadePixPhysicsList("QGSP_BERT");

    }
    else if (name == "TestXR_Compton_and_Photoelectric") {

        G4cout << "[PHYS] This a reduced physics list which only includes Compton scattering and Photoelectric effect for photons" << G4endl;
        delete emJadePixPhysicsList;
        //emJadePixPhysicsList = new G4EmCompPhotoEPhysics();

    }

    else {


        G4cout << "Physics list not found"
            << G4endl;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetBuilderList0(G4bool flagHP)
{
    hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    if(flagHP) {
        hadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
        hadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }
    //hadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
    hadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
    hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetBuilderList1(G4bool flagHP)
{
    hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    if(flagHP) {
        hadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
        hadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }
    hadronPhys.push_back( new G4IonPhysics(verboseLevel));
    hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetBuilderList2(G4bool /*addStopping*/)
{
    hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    hadronPhys.push_back( new G4IonPhysics(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetBuilderList3()
{
    hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    RegisterPhysics( new G4HadronElasticPhysicsXS(verboseLevel) );
    hadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
    hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetBuilderList4()
{
    hadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    hadronPhys.push_back( new G4IonPhysics(verboseLevel));
    hadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetCuts()
{

    if (verboseLevel >0){
        G4cout << "JadePixPhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");
    SetCutValue(cutForProton, "proton");

    if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void JadePixPhysicsList::SetCutForGamma(G4double cut)
{
    cutForGamma = cut;
    SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixPhysicsList::SetCutForElectron(G4double cut)
{
    cutForElectron = cut;
    SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixPhysicsList::SetCutForPositron(G4double cut)
{
    cutForPositron = cut;
    SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixPhysicsList::SetCutForProton(G4double cut)
{
    cutForProton = cut;
    SetParticleCuts(cutForProton, G4Proton::Proton());
}

void JadePixPhysicsList::List()
{
    G4cout << "### PhysicsLists available: FTFP_BERT FTFP_BERT_EMV FTFP_BERT_EMX FTFP_BERT_EMY FTF_BIC"
        << G4endl;
    G4cout << "                            LHEP LHEP_EMV QBBC QGS_BIC QGSP"
        << G4endl;
    G4cout << "                            QGSC_BERT QGSP_BERT QGSP_BERT_EMV QGSP_BIC_EMY"
        << G4endl;
    G4cout << "                            QGSP_BERT_EMX QGSP_BERT_HP QGSP_BIC QGSP_BIC_HP LIVERMORE_LIVERMORE_FTFP_BERT"
        << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

