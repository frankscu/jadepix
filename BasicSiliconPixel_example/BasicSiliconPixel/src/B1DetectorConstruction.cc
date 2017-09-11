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
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "B1SensitiveDetector.hh"
#include "G4SDManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fLogicShape(NULL)
{ 
  //fNbofShapes = 16*16;
  fNbofShapes = 1*1;
  fLogicShape = new G4LogicalVolume*[fNbofShapes]; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{
  delete [] fLogicShape;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXZ = 1*cm, env_sizeY = 3*cm;
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXZ = 1.2*env_sizeXZ;
  G4double world_sizeY  = 1.2*env_sizeY;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =    
      new G4Box("World",                       //its name
              0.5*world_sizeXZ, 0.5*world_sizeY, 0.5*world_sizeXZ);     //its size

  G4LogicalVolume* logicWorld =                         
      new G4LogicalVolume(solidWorld,          //its solid
              world_mat,           //its material
              "World");            //its name

  G4VPhysicalVolume* physWorld = 
      new G4PVPlacement(0,                     //no rotation
              G4ThreeVector(),       //at (0,0,0)
              logicWorld,            //its logical volume
              "World",               //its name
              0,                     //its mother  volume
              false,                 //no boolean operation
              0,                     //copy number
              checkOverlaps);        //overlaps checking

  //     
  // Shape 
  //  
  G4Material* shape_mat = nist->FindOrBuildMaterial("G4_Si");
  G4double pixelsize = 0.055*mm;
  G4double pitch = 0.055*mm;

  G4double shape_pX =  0.055*mm;
  G4double shape_pY =  0.3*mm;
  G4double shape_pZ = 	0.055*mm;

  G4double firstXposition = -pixelsize*sqrt(fNbofShapes)/2;
  G4double firstYposition = 0;
  G4double firstZposition = -pixelsize*sqrt(fNbofShapes)/2;

  // Conical section shape       

  G4int copyNo = 0;
  G4double Xposition =  firstXposition;
  G4double Yposition =  firstYposition;
  G4double Zposition =  firstZposition;
  for(G4int copyNoX=0; copyNoX<sqrt(fNbofShapes); copyNoX++)
      for(G4int copyNoZ=0; copyNoZ<sqrt(fNbofShapes); copyNoZ++)
      {		
          Xposition = firstXposition + copyNoX*pitch;
          Zposition = firstZposition + copyNoZ*pitch;	
          G4ThreeVector pos = G4ThreeVector(Xposition,Yposition,Zposition);  
          G4Box* solidShape =    
              new G4Box("Shape", 
                      shape_pX, shape_pY,shape_pZ);

          fLogicShape[copyNo] =                         
              new G4LogicalVolume(solidShape,         //its solid
                      shape_mat,          //its material
                      "ShapeLV",          //its name
                      0,0,0);                 
          new G4PVPlacement(0,                     //no rotation
                  pos,                    //at position
                  fLogicShape[copyNo],    //its logical volume
                  "ShapePV",             //its name
                  logicWorld,             //its mother  volume
                  false,                  //no boolean operation
                  copyNo,                 //copy number
                  checkOverlaps);	 //checking overlaps
          copyNo++;
      }

  //
  //always return the physical World
  //
  return physWorld;
}

void B1DetectorConstruction::ConstructSDandField()
{
  SD = new B1SensitiveDetector("ShapeSD");

  G4SDManager* SDManager = G4SDManager::GetSDMpointer(); 
  SDManager->AddNewDetector(SD);
  
  SetSensitiveDetector("ShapeLV",SD);

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
