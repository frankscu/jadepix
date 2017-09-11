#ifndef B1PrimaryGeneratorAction_GPS_h
#define B1PrimaryGeneratorAction_GPS_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class G4Box;

class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  B1PrimaryGeneratorAction();
  ~B1PrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);
  G4GeneralParticleSource* GetParticleGun() const { return fParticleGun;};

private:
  G4GeneralParticleSource* fParticleGun;
  //G4Box* fEnvelopeBox;
  
};

#endif
