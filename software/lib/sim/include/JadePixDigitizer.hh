#ifndef JadePixDigitizer_H
#define JadePixDigitizer_H 

class JadePixDigitizerMessenger;
class G4Svc;

#include "G4VDigitizerModule.hh"
#include "JadePixIdentifier.hh"
#include "JadePixHit.hh"
#include "JadePixDigi.hh"
#include "JadePixGeoParameter.hh"

#include "globals.hh"
#include <vector>


class JadePixDigitizer : public G4VDigitizerModule
{
    public:
        JadePixDigitizer(G4String modName);
        ~JadePixDigitizer();

        virtual void Digitize();
        void AddFake(void);

        int GetADC(double edep);
        int GetVolADC(double edep);
        int GetTDC(double time);
        int InOrNot(int poly_sides, double *poly_x, double *poly_y, double x, double y);

        void HitRealizition(JadePixHit* rawHit);
        void DiffuseE(G4double edep,G4ThreeVector hitPoint,JadePixIdentifier& JadePixId,JadePixHit* rawHit);
        void DiffuseGaussE(G4double edep,G4ThreeVector hitPoint,JadePixIdentifier& JadePixId,JadePixHit* rawHit);

        void HitRealizitionEelectrode(JadePixHit* rawHit);
        double DiffuseGaussElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint);
        double DiffuseGaussElectrodeDivided(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint);
        double DiffuseGaussLorentzElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint);
        double OverMOSDiffuseGaussLorentzElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint);

        void PixelIntegration(G4double ePix,JadePixIdentifier& JadePixId,JadePixHit* rawHit);

    private:


        std::vector<JadePixHit*> realizedHitsCol;
        std::map<unsigned int,G4int> realizedHitMap;
        std::map<unsigned int,G4int>::iterator itRealizedHitMap;

        JadePixDigisCollection* digisCollection;         
        JadePixGeoParameter* JadePixGeo;

        std::map<G4long,G4int> digiMap;
        std::map<G4long,G4int>::iterator itDigiMap;

        G4double pedestal ;  // e-
        G4double enc ;  // e- ENC
        G4double cce ;  // charge collection efficiency
        //G4double adcRange;
        G4double ehpEnergy;
        G4double energyThreshold;

        G4double adcEnergyRange;
        G4int nAdcBit;

        G4double noiseFlag; //1: Add electronic noise; 0: do nothing
        G4int fakeHitFlag; //1: Add fake hit; 0: do nothing
};

#endif

