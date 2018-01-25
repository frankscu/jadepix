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
/// \file JadePixHit.cc
/// \brief Implementation of the JadePixHit class

#include "JadePixHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<JadePixHit> JadePixHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//JadePixHit::JadePixHit() {}

JadePixHit::JadePixHit()
 : G4VHit(),
   trackID(-9999),
   layerID(-9999),
   ladderID(-9999),
   chipID(-9999),
   globalChipID(-9999),
   rowID(-9999),
   columnID(-9999),
   edep(0.),
   globalT(0.),
   pos(G4ThreeVector(0.,0.,0.)),
   theta(0.),
   enterAngle(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixHit::~JadePixHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JadePixHit::JadePixHit(const JadePixHit& right)
  : G4VHit()
{
  trackID   = right.trackID;
  layerID   = right.layerID;
  ladderID  = right.ladderID;
  chipID    = right.chipID;
  globalChipID    = right.globalChipID;
  rowID    = right.rowID;
  columnID    = right.columnID;
  edep      = right.edep;
  globalT   = right.globalT;
  pos       = right.pos;
  theta     = right.theta;
  enterAngle= right.enterAngle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const JadePixHit& JadePixHit::operator=(const JadePixHit& right)
{
  trackID   = right.trackID;
  layerID   = right.layerID;
  ladderID  = right.ladderID;
  chipID    = right.chipID;
  globalChipID    = right.globalChipID;
  rowID    = right.rowID;
  columnID    = right.columnID;
  edep      = right.edep;
  globalT   = right.globalT;
  pos       = right.pos;
  theta     = right.theta;
  enterAngle= right.enterAngle;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int JadePixHit::operator==(const JadePixHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JadePixHit::Print()
{
   G4cout << "  trackID: " << trackID 
     << "  layerID: " << layerID
     << "  ladderID:  " << ladderID
     << "  chipID:  " << chipID
     << "  row:  " << rowID
     << "  column:  " << columnID
     << "  energy deposit: " << G4BestUnit(edep,"Energy")
     << "  position: " << G4BestUnit(pos,"Length") 
     << "  theta: " << G4BestUnit(theta,"Angle")
     << "  enterAngle: " << G4BestUnit(enterAngle,"Angle")
     <<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
