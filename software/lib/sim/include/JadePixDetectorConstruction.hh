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
/// \file JadePixDetectorConstruction.hh
/// \brief Definition of the JadePixDetectorConstruction class

#ifndef JadePixDetectorConstruction_h
#define JadePixDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "JadePixGeoParameter.hh"
#include "globals.hh"


class JadePixDetectorMessenger;
class G4Box;
class G4VPhysicalVolume;
class G4UniformMagField;
class G4UserLimits;

/// Detector construction class to define materials and geometry.
/// The calorimeter is a box made of a given number of layers. A layer consists
/// of an absorber plate and of a detection gap. The layer is replicated.
///
/// Four parameters define the geometry of the calorimeter :
///
/// - the thickness of an absorber plate,
/// - the thickness of a gap,
/// - the number of layers,
/// - the transverse size of the calorimeter (the input face is a square).
///
/// In DefineVolumes(), sensitive detectors of JadePixSD type
/// are created and associated with the Absorber and Gap volumes.
///
/// In addition a transverse uniform magnetic field is defined in
/// SetMagField() method which can be activated
/// via a command defined in the JadePixDetectorMessenger class. 

class JadePixDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    JadePixDetectorConstruction();
    virtual ~JadePixDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

    // set methods
    //
    void SetMagField(G4double fieldValue);

    void SetMaxStep (G4double );

  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
 
    JadePixGeoParameter* JadePixGeo;

    // data members
    //
    JadePixDetectorMessenger*  fMessenger; // messenger 
    G4UniformMagField*     fMagField;  // magnetic field
    
    G4UserLimits* fStepLimit; //pointer to user step limits

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

