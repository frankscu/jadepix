#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "B1PrimaryGeneratorAction.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
{
  fParticleGun = new G4GeneralParticleSource ();
}

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  fParticleGun->GeneratePrimaryVertex(anEvent);
}


