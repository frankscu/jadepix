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
// The code was written by :
//  ^Claudio Andenna  claudio.andenna@ispesl.it, claudio.andenna@iss.infn.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//  with the contribute of Alessandro Occhigrossi*
//
// ^INAIL DIPIA - ex ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//


#include "JadePixPrimaryGeneratorActionMessenger.hh"
#include "JadePixPrimaryGeneratorAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandExponential.h"

//#include "TFile.h"
//#include "TH1.h"
using namespace CLHEP;

JadePixPrimaryGeneratorActionMessenger::JadePixPrimaryGeneratorActionMessenger(JadePixPrimaryGeneratorAction *PJadePixPrimaryGeneratorAction) : pJadePixPrimaryGeneratorAction(PJadePixPrimaryGeneratorAction)
{

  //m_hits          = 1;
  //m_frames        = 1;
  //m_beamTypeHitFunc = "gauss";
  //m_beamTypePar1  = 1.;
  //m_beamTypePar2  = 1.;

  //particleType=new G4UIcmdWithAString("/jadepix/beam/particle",this);
  //particleType->SetDefaultValue("mu-");
  //particleType->SetGuidance("type of particle generator source  (randomTarget, phaseSpace)");

  //beamNumberOfFramesCmd = new G4UIcmdWithAnInteger("/jadepix/beam/frames",this);
  //beamNumberOfFramesCmd->SetGuidance("Set number of frames");
  //beamNumberOfFramesCmd->SetParameterName("Frames",false,false);
  //beamNumberOfFramesCmd->SetDefaultValue(1);
  //beamNumberOfFramesCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //beamOnCmd = new G4UIcmdWithoutParameter("/jadepix/beam/on",this);
  //beamOnCmd->SetGuidance("Set beam ON");
  //beamOnCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  //beamTypeCmd =new G4UIcommand("/jadepix/beam/type",this);
  //beamTypeCmd->SetGuidance("Select hits distribution function.");
  //beamTypeCmd->SetGuidance("Current possible functions:");
  //beamTypeCmd->SetGuidance("const <hits>");
  //beamTypeCmd->SetGuidance("gauss <mean> <sigma>");
  //beamTypeCmd->SetGuidance("poisson <mean>");
  //beamTypeCmd->SetGuidance("expo <tau>");
  //beamTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  //G4UIparameter * func = new G4UIparameter("hitFunction",'s',true);
  //func->SetDefaultValue("const");
  //beamTypeCmd->SetParameter(func);
  //G4UIparameter * p1 = new G4UIparameter("par1",'d',true);
  //p1->SetDefaultValue(1.);
  //p1->SetParameterRange("par1 >= 0");
  //beamTypeCmd->SetParameter(p1);
  //G4UIparameter * p2 = new G4UIparameter("par2",'d',true);
  //p2->SetDefaultValue(1.);
  //beamTypeCmd->SetParameter(p2);
  //p2->SetParameterRange("par2 > 0");


  //gunEnergy=new G4UIcmdWithADoubleAndUnit("/jadepix/beam/energy", this);
  //gunEnergy->SetDefaultUnit("GeV");
  //gunEnergy->SetDefaultValue(20.);
  //gunEnergy->SetGuidance("mean energy of the primary particles");

  gunPosXYSpread=new G4UIcmdWithADoubleAndUnit("/jadepix/beam/posXYSpread", this);
  gunPosXYSpread->SetDefaultUnit("mm");
  gunPosXYSpread->SetDefaultValue(1.);
  gunPosXYSpread->SetGuidance("X-Y position of the gun");

  gunPosZ=new G4UIcmdWithADoubleAndUnit("/jadepix/beam/posZ", this);
  gunPosZ->SetDefaultUnit("mm");
  gunPosZ->SetDefaultValue(1.);
  gunPosZ->SetGuidance("Z position of the gun");

}

JadePixPrimaryGeneratorActionMessenger::~JadePixPrimaryGeneratorActionMessenger(void)
{
  delete gunPosXYSpread;
  delete gunPosZ;
  //delete gunEnergy;
  //delete particleType;
  //delete beamTypeCmd;
  //delete beamNumberOfFramesCmd;
  //delete beamOnCmd;
}
void JadePixPrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{

  //if (cmd==particleType){
  //    pJadePixPrimaryGeneratorAction->SetParticleType(newValue);
  //}

  //if (cmd==gunEnergy)
  //{
  //  gunEnergy->GetNewUnitValue(newValue);
  //  pJadePixPrimaryGeneratorAction->SetGunEnergy(gunEnergy->GetNewDoubleValue(newValue));
  //}

  if (cmd==gunPosXYSpread)
  {
    gunPosXYSpread->GetNewUnitValue(newValue);
    pJadePixPrimaryGeneratorAction->SetGunPosXYSpread(gunPosXYSpread->GetNewDoubleValue(newValue));
  }

  if (cmd==gunPosZ)
  {
    gunPosZ->GetNewUnitValue(newValue);
    pJadePixPrimaryGeneratorAction->SetGunPosZ(gunPosZ->GetNewDoubleValue(newValue));
  }

  //if (cmd==beamNumberOfFramesCmd)
  //{
  //  m_frames = beamNumberOfFramesCmd->GetNewIntValue(newValue);
  //}

  //if (cmd==beamTypeCmd)
  //{
  //  const char* nv = (const char*)newValue;
  //  std::istringstream is(nv);
  //  is >> m_beamTypeHitFunc  >> m_beamTypePar1 >> m_beamTypePar2;
  //}

  //if (cmd==beamOnCmd)
  //{
  //  G4RunManager * runManager = G4RunManager::GetRunManager();
  //  
  //  // Write number of hits sent to check the distribution
  //  //TFile * f = new TFile("hitFunction.root","recreate");
  //  //TH1I * h = new TH1I("h","h",1000,0,1000);

  //  for (G4int f = 0; f < m_frames; f++)
  //  {
  //    if ( m_beamTypeHitFunc == "gauss" )
  //    {
  //      m_hits = CLHEP::RandGauss::shoot(m_beamTypePar1,m_beamTypePar2);
  //    }
  //    else if ( m_beamTypeHitFunc == "poisson" )
  //    {
  //      m_hits = CLHEP::RandPoisson::shoot(m_beamTypePar1);
  //    }
  //    else if ( m_beamTypeHitFunc == "expo" )
  //    {
  //      m_hits = CLHEP::RandExponential::shoot(m_beamTypePar1);
  //    }
  //    else if ( m_beamTypeHitFunc == "const")
  //    {
  //      m_hits = m_beamTypePar1;
  //    }
  //    else
  //    {
  //      G4cout << "============> Unknown parameter: " <<  m_beamTypeHitFunc  << "... sending "<< m_hits << " hit(s)..." << G4endl;
  //    }

  //    // same call as in /run/beamOn
  //    runManager->BeamOn(m_hits); //Fix it. Run each time, the write file crashed.
  //    //h->Fill(m_hits);
  //  }
  //  //f->Write();
  //  //f->Close();
  //}
}
