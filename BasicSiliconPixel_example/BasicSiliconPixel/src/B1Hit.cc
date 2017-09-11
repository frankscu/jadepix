#include "B1Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<B1Hit>* B1HitAllocator=0;

B1Hit::B1Hit()
 :  G4VHit(), 
    fEdep(0.),
    fPos(G4ThreeVector()),
    fTrackID(-1),
    fShapeNb(-1),
    fPolarization(G4ThreeVector())
{}

B1Hit::~B1Hit() {}

B1Hit::B1Hit(const B1Hit& right)
  : G4VHit()
{
  fEdep      = right.fEdep;
  fShapeNb   = right.fShapeNb;
  fPos       = right.fPos;
  fPolarization = right.fPolarization;
  fTrackID   = right.fTrackID;

}

const B1Hit& B1Hit::operator=(const B1Hit& right)
{
  fEdep      = right.fEdep;
  fShapeNb   = right.fShapeNb;
  fPos       = right.fPos;
  fPolarization = right.fPolarization;
  fTrackID   = right.fTrackID;

  return *this;
}

G4int B1Hit::operator==(const B1Hit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

void B1Hit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void B1Hit::Print()
{
  G4cout
     << "\n-----------------------------------------------------\n"
     << "TrackID: "
     << fTrackID 
     << "  ShapeNb: "
     << fShapeNb    
     << " Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit(fPos,"Length")
     << " Polarization:"
     << std::setw(7) << fPolarization
     << "\n-----------------------------------------------------\n"
     << G4endl;
}


