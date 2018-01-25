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
// $Id$
// 
/// \file JadePixDetectorMessenger.cc
/// \brief Implementation of the JadePixDetectorMessenger class

#include "JadePixDetectorMessenger.hh"
#include "JadePixDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixDetectorMessenger::JadePixDetectorMessenger(JadePixDetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{ 
  fJadePixDirectory = new G4UIdirectory("/jadepix/");
  fJadePixDirectory->SetGuidance("UI commands of this example");
  
  fDetDirectory = new G4UIdirectory("/jadepix/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fSetMagFieldCmd = new G4UIcmdWithADoubleAndUnit("/jadepix/det/setMagField",this);  
  fSetMagFieldCmd->SetGuidance("Define magnetic field.");
  fSetMagFieldCmd->SetGuidance("Magnetic field will be in X direction.");
  fSetMagFieldCmd->SetParameterName("Bx",false);
  fSetMagFieldCmd->SetUnitCategory("Magnetic flux density");
  fSetMagFieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/jadepix/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixDetectorMessenger::~JadePixDetectorMessenger()
{
  delete fSetMagFieldCmd;
  delete fDetDirectory;
  delete fJadePixDirectory;  
  delete fStepMaxCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fSetMagFieldCmd ) { 
    fDetectorConstruction
      ->SetMagField(fSetMagFieldCmd->GetNewDoubleValue(newValue));
  }

  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
