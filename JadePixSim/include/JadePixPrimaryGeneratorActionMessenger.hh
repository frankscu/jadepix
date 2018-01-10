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


#ifndef JadePixPrimaryGeneratorActionMessengerH
#define JadePixPrimaryGeneratorActionMessengerH


#include "globals.hh"
#include "G4UImessenger.hh"

class JadePixPrimaryGeneratorAction;
class G4UImessenger;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcommand;

class JadePixPrimaryGeneratorActionMessenger : public G4UImessenger
{
  public:
    JadePixPrimaryGeneratorActionMessenger(JadePixPrimaryGeneratorAction *PJadePixPrimaryGeneratorAction);
    ~JadePixPrimaryGeneratorActionMessenger(void);
    void SetNewValue(G4UIcommand* cmd, G4String newValue);

  private:
    JadePixPrimaryGeneratorAction *pJadePixPrimaryGeneratorAction;

    G4UIcmdWithADoubleAndUnit     *gunPosXYSpread;
    G4UIcmdWithADoubleAndUnit     *gunPosZ;
    G4UIcmdWithADoubleAndUnit     *gunEnergy;
    G4UIcmdWithAString            *particleType;
    G4UIcmdWithAnInteger          *beamNumberOfFramesCmd;
    G4UIcommand                   *beamTypeCmd;
    G4UIcmdWithoutParameter       *beamOnCmd;

    G4int m_hits;
    G4int m_frames;
    G4String m_beamTypeHitFunc;
    G4double m_beamTypePar1;
    G4double m_beamTypePar2;
};

#endif

