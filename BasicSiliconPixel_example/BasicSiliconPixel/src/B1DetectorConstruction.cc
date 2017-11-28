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

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
//  fLogicShape(NULL) 
  fLogicShape(nullptr),
  fVisAttributes()
{ 
  //fNbofShapes = 16*16;
  fNbofShapes = 5*5;
  fLogicShape = new G4LogicalVolume*[fNbofShapes]; 
  //fLogicShape = new G4LogicalVolume(fNbofShapes); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{
  delete [] fLogicShape;

  for (auto visAttributes: fVisAttributes) {
    delete visAttributes;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  //G4double env_sizeXZ = 1*cm, env_sizeY = 3*cm;
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

//  G4Box* solidWorld =    
//      new G4Box("World",                       //its name
//              0.5*world_sizeXZ, 0.5*world_sizeY, 0.5*world_sizeXZ);     //its size

  G4Box* solidWorld =    
      new G4Box("World",                       //its name
              10.*m, 10.*m, 10.*m);     //its size


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
  G4double pixelsize = 0.003*mm;
  G4double gap = 0.003*mm;
  G4double gappluspixelsize = pixelsize*2 + gap;


  G4double shape_pX =  0.003*mm;
  G4double shape_pY =  0.003*mm;
  G4double shape_pZ =  0.003*mm;

  G4double firstXposition = -gappluspixelsize*(sqrt(fNbofShapes-1))/2;
  G4double firstYposition = 0;
  G4double firstZposition = -gappluspixelsize*(sqrt(fNbofShapes-1))/2;

  // Conical section shape       

  G4int copyNo = 0;
  G4double Xposition =  firstXposition;
  G4double Yposition =  firstYposition;
  G4double Zposition =  firstZposition;

  auto solidShape =
      new G4Box("Shape",
              shape_pX, shape_pY,shape_pZ);
  //G4LogicalVolume* fLogicShape =



  for(G4int copyNoX=0; copyNoX<sqrt(fNbofShapes); copyNoX++)
      for(G4int copyNoZ=0; copyNoZ<sqrt(fNbofShapes); copyNoZ++)
      {		
          Xposition = firstXposition + gappluspixelsize*copyNoX;
          Zposition = firstZposition + gappluspixelsize*copyNoZ;
          G4ThreeVector pos = G4ThreeVector(Xposition,Yposition,Zposition);  

      fLogicShape[copyNo] =
          new G4LogicalVolume(solidShape, shape_mat, "ShapeLV",0,0,0);

          new G4PVPlacement(0, pos, fLogicShape[copyNo], "ShapePV", logicWorld, false, copyNo, checkOverlaps);
          copyNo++;
      }

  G4double z;
  G4double a;
  G4double density; 
  G4int nel;
  G4int natoms;

  G4Element* O  = new G4Element("Oxygen" , "O", z=8., a= 16.00*g/mole);
  G4Element* Si = new G4Element("Sillicon", "Si", z=14., a= 28.00*g/mole);
  G4Material* SiO2 = new G4Material("SiO2", density= 2.2*g/cm3, nel=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O , natoms=2);

  G4double belt_sizeY=0.02*mm;
  G4double belt_sizeZ=0.003*mm;
  G4double belt_sizeX=0.2*mm;

//
//belt1
//
  G4Box* solidbelt1 =
      new G4Box("belt1", belt_sizeX-belt_sizeY/4., belt_sizeY, belt_sizeZ);

  G4LogicalVolume* logicbelt1 =
      new G4LogicalVolume(solidbelt1, SiO2, "belt1");
//  G4VPhysicalVolume* physbelt1 =
  new G4PVPlacement(0,G4ThreeVector(0.,-0.007*mm,-0.2*mm), logicbelt1, "belt1",logicWorld,false,0,checkOverlaps);

//
//belt2
//
  G4Box* solidbelt2 =
      new G4Box("belt2", belt_sizeZ, belt_sizeY, belt_sizeX);

  G4LogicalVolume* logicbelt2 =
      new G4LogicalVolume(solidbelt2, SiO2, "belt2");
//  G4VPhysicalVolume* physbelt2 =
  new G4PVPlacement(0,G4ThreeVector(-0.2*mm,-0.007*mm,0.), logicbelt2, "belt2",logicWorld,false,0,checkOverlaps);


//
//belt3
//
  G4Box* solidbelt3 =
      new G4Box("belt3", belt_sizeX-belt_sizeY/4., belt_sizeY, belt_sizeZ);

  G4LogicalVolume* logicbelt3 =
      new G4LogicalVolume(solidbelt3, SiO2, "belt3");
//  G4VPhysicalVolume* physbelt3 =
  new G4PVPlacement(0,G4ThreeVector(0.,-0.007*mm,0.2*mm), logicbelt3, "belt3",logicWorld,false,0,checkOverlaps);


//
//belt4
//
  G4Box* solidbelt4 =
      new G4Box("belt4", belt_sizeZ, belt_sizeY, belt_sizeX);

  G4LogicalVolume* logicbelt4 =
      new G4LogicalVolume(solidbelt4, SiO2, "belt4");
//  G4VPhysicalVolume* physbelt4 =
  new G4PVPlacement(0,G4ThreeVector(0.2*mm,-0.007*mm,0.), logicbelt4, "belt4",logicWorld,false,0,checkOverlaps);

//
//substrate
//

  G4Material* substrate_mat = nist->FindOrBuildMaterial("G4_Si");

  G4double substrate_sizeX = 0.2*mm;
  G4double substrate_sizeY = 0.015*mm;
  G4double substrate_sizeZ = 0.2*mm; 

  G4Box* solidsubstrate =
      new G4Box("substrate", substrate_sizeX, substrate_sizeY, substrate_sizeZ);

  G4LogicalVolume* logicsubstrate =
      new G4LogicalVolume(solidsubstrate, substrate_mat, "substrate");
//  G4VPhysicalVolume* physbelt4 =
  new G4PVPlacement(0,G4ThreeVector(0.,(0.003-0.02-0.003-0.01-0.015)*mm,0.), logicsubstrate, "substrate",logicWorld,false,0,checkOverlaps);

  // visualization attributes ------------------------------------------------
  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

//  copyNo= 0;
//  for(G4int copyNoX=0; copyNoX<sqrt(fNbofShapes); copyNoX++)
//      for(G4int copyNoZ=0; copyNoZ<sqrt(fNbofShapes); copyNoZ++)
//      {
//         visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   
//         fLogicShape[copyNo]->SetVisAttributes(visAttributes);
//         copyNo++;
//      }
//  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  logicbelt1->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  logicbelt2->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  logicbelt3->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  logicbelt4->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
  logicsubstrate->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

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
  G4int copyNo= 0;
  for(G4int copyNoX=0; copyNoX<sqrt(fNbofShapes); copyNoX++)
       for(G4int copyNoZ=0; copyNoZ<sqrt(fNbofShapes); copyNoZ++)
       {
         fLogicShape[copyNo]->SetSensitiveDetector(SD);
         copyNo++;
       }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
