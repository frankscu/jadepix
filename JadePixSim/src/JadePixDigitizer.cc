//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description:
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: Oct. 26, 2004
//Modified:
//Comment:
//---------------------------------------------------------------------------//

#include "JadePixDigitizer.hh"
//#include "JadePixDigitizerMessenger.hh"
#include "JadePixHit.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4DigiManager.hh"
#include "Randomize.hh"
#include "G4ios.hh"
#include <string>
#include <math.h>

JadePixDigitizer::JadePixDigitizer(G4String modName):G4VDigitizerModule(modName){

  JadePixGeo=JadePixGeoParameter::GetGeo();

  collectionName.push_back("JadePixDigisCollection");

  noiseFlag = 1.;
  fakeHitFlag = 0;

  nAdcBit = JadePixGeo->Layer(0).ADCBitNo();
  cce = JadePixGeo->Layer(0).CCE();

  enc = JadePixGeo->Layer(0).ENC();

  pedestal = 0;
  ehpEnergy = 3.6 *eV;
  //energyThreshold = (pedestal+enc)*ehpEnergy;
  energyThreshold = 0;
  adcEnergyRange = JadePixGeo->Layer(0).ADCRange()*ehpEnergy;
}

JadePixDigitizer::~JadePixDigitizer(){}

void JadePixDigitizer::Digitize(){

  digiMap.clear();

  G4int  NHits;
  G4double pixEdep;


  G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();

  //hits collection ID
  G4int THCID=-1;
  THCID = DigiMan->GetHitsCollectionID("JadePixHitsCollection");

  //hits collection
  JadePixHitsCollection* THC = 0;
  THC = (JadePixHitsCollection*) (DigiMan->GetHitsCollection(THCID));

  if(THC){
    digisCollection=new JadePixDigisCollection(moduleName, collectionName[0]);
    NHits=THC->entries();

    //G4cout<<"BesSimDigitizer::Primary Hits No: "<<NHits<<endl;
    for(G4int i=0;i<NHits;i++){
      //HitRealizition((*THC)[i]);
      //G4cout<<"Total Edep: "<<(*THC)[i]->GetEdep()<<", ehPairs: "<<(*THC)[i]->GetEdep()/ehpEnergy<<G4endl;
      HitRealizitionEelectrode((*THC)[i]);
    }


    G4int nrHit=0;
    std::vector<JadePixHit*>::iterator itRealHitCol;
    for(itRealHitCol=realizedHitsCol.begin();itRealHitCol!=realizedHitsCol.end();++itRealHitCol){
      JadePixHit* _realHit = *itRealHitCol;
      pixEdep = _realHit->GetEdep() + noiseFlag*ehpEnergy*G4RandGauss::shoot(pedestal,enc);
      if(pixEdep>energyThreshold){
        JadePixDigi* newDigi = new JadePixDigi();
        newDigi->SetTrackID(_realHit->GetTrackID());
        newDigi->SetLayerID(_realHit->GetLayerID());
        newDigi->SetLadderID(_realHit->GetLadderID());
        newDigi->SetChipID(_realHit->GetChipID());
        newDigi->SetGlobalChipID(_realHit->GetGlobalChipID());
        newDigi->SetRow(_realHit->GetRow());
        newDigi->SetCol(_realHit->GetCol());
        //newDigi->SetEdep(pixEdep);
        newDigi->AddEdep(pixEdep);
        //std::cout<<"Edep: "<<G4BestUnit(pixEdep,"Energy")<<endl;
        newDigi->SetGlobalT(_realHit->GetGlobalT());
        //int adc = GetADC(pixEdep);
        int adc = GetVolADC(pixEdep); //Change ADC method
        int tdc = GetTDC(_realHit->GetGlobalT());
        newDigi->SetADC(adc);
        newDigi->SetTDC(tdc);
        JadePixIdentifier _JadePixDigiId(_realHit->GetLayerID(),_realHit->GetLadderID(),_realHit->GetChipID(),_realHit->GetCol(),_realHit->GetRow());
        unsigned int key=_JadePixDigiId.PixelID();
        G4int NbDigis = digisCollection->insert(newDigi);
        digiMap[key]=NbDigis-1;
        nrHit++;
        if(verboseLevel>0) {
          G4cout<<"MyMessage::Processing Digi: "<<" layer: "<<_realHit->GetLayerID()<<" ladder: "<<_realHit->GetLadderID()<<" Chip: "<<_realHit->GetChipID()<<" Row: "<<_realHit->GetRow()<<" Col: "<<_realHit->GetCol()<<" Edep: "<<_realHit->GetEdep()<<", ehPairs: "<<_realHit->GetEdep()/ehpEnergy<<", ADC: "<<adc<<G4endl;
        }
      }
      delete _realHit;
    }
    realizedHitsCol.clear();
    realizedHitMap.clear();

    //G4cout<<"MyMessage::Primary Hit No: "<<NHits<<" Realized Hit No: "<<nrHit<<G4endl;
    if(fakeHitFlag==1) AddFake();

    if (verboseLevel>0) {
      G4cout << "\n-------->digis Collection: in this event they are "
        << digisCollection->entries()
        << " digis in the JadePix: " << G4endl;
      digisCollection->PrintAllDigi();
    }
    StoreDigiCollection(digisCollection);
  }

}

void JadePixDigitizer::HitRealizition(JadePixHit* rawHit){

  G4int layerId = rawHit->GetLayerID();
  G4int ladderId = rawHit->GetLadderID();
  G4int chipId = rawHit->GetChipID();
  JadePixIdentifier JadePixId(layerId,ladderId,chipId,-1,-1);

  G4ThreeVector locInPos = rawHit->GetPrePos();
  G4ThreeVector locOutPos = rawHit->GetPostPos();

  G4double edep = rawHit->GetEdep();

  G4ThreeVector locMidPos = (locInPos+locOutPos)/2;

  // Change to Possion Distribution
  G4double preEdep=edep*(0.23+0.2*G4UniformRand());
  G4double postEdep=edep*(0.23+0.2*G4UniformRand());
  G4double midEdep=edep-preEdep-postEdep;
  //G4cout<<"MyMessage::Edep: "<<edep<<" PreEdep: "<<preEdep<<" PostEdep: "<<postEdep<<" MidEdep: "<<midEdep<<G4endl;
  G4int digiMethod = JadePixGeo->Layer(0).DigiMethod();

  if(digiMethod == 0){
    if (verboseLevel>0){ G4cout<<"Two Gauss is in use!" << G4endl;}
    DiffuseE(preEdep,locInPos,JadePixId,rawHit);
    DiffuseE(midEdep,locMidPos,JadePixId,rawHit);
    DiffuseE(postEdep,locOutPos,JadePixId,rawHit);
  }else if(digiMethod == 1){
    if(verboseLevel>0){G4cout<<"Gauss is in use!" << G4endl;}
    DiffuseGaussE(preEdep,locInPos,JadePixId,rawHit);
    DiffuseGaussE(midEdep,locMidPos,JadePixId,rawHit);
    DiffuseGaussE(postEdep,locOutPos,JadePixId,rawHit);
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void JadePixDigitizer::DiffuseE(G4double edep,G4ThreeVector hitPoint,JadePixIdentifier& JadePixId,JadePixHit* rawHit){

  //G4cout<<"MyMessage::locHitPosX: "<<hitPoint.x()<<" locHitPosY: "<<hitPoint.y()<<" locHitPosZ: "<<hitPoint.z()<<G4endl;
  G4double sigmaS = 1*um;
  G4double sigmaL = 2.5*um;
  G4double frac=0.8;
  G4double diffuseSize = 5*sigmaL;
  G4int nSector=10;
  G4double secPitch = diffuseSize/(2*nSector+1);
  G4double secEdep;
  G4double eT=0;
  for(G4int iSC=-nSector;iSC<nSector+1;++iSC){
    for(G4int iSR=-nSector;iSR<nSector+1;++iSR){
      G4double dx=secPitch*iSC;
      G4double dy=secPitch*iSR;
      G4double iPosX=hitPoint.x()+dx;
      G4double iPosY=hitPoint.y()+dy;
      //gauss pdf
      G4double pdf1 = exp(-(pow(dx,2)+pow(dy,2))/(2*sigmaS*sigmaS))/(twopi*sigmaS*sigmaS);
      G4double pdf2 = exp(-(pow(dx,2)+pow(dy,2))/(2*sigmaL*sigmaL))/(twopi*sigmaL*sigmaL);

      //exp pdf
      //G4double r=sqrt(pow(iX-hitPoint.x(),2)+pow(iY-hitPoint.y(),2));
      //if(r<=sigma/3) r=sigma/3;
      // G4double pdf = exp(-r/sigma)/(twopi*r*sigma);
      //G4double xy=fabs(dx)+fabs(dy);
      //if(xy<secPitch) xy=secPitch*4/7;
      //G4double pdf2=exp(-xy/sigmaL)/(4*sigmaL*sigmaL);

      G4double pdf= frac*pdf1 + (1-frac)*pdf2;
      secEdep = edep*pdf*secPitch*secPitch;
      //G4cout<<"MyMessage::locCol: "<<iSC<<" locRow: "<<iSR<<" Edep: "<<secEdep<<G4endl;
      JadePixId.WhichPixel(iPosX,iPosY);
      PixelIntegration(secEdep,JadePixId,rawHit);
      eT += secEdep;
    }
  }

  //G4cout<<"MyMessage::Total E Collect: "<<eT<<" Total Percent: "<<100*eT/edep<<"%"<<G4endl;
}


void JadePixDigitizer::DiffuseGaussE(G4double edep,G4ThreeVector hitPoint,JadePixIdentifier& JadePixId,JadePixHit* rawHit){

  //G4cout<<"MyMessage::locHitPosX: "<<hitPoint.x()<<" locHitPosY: "<<hitPoint.y()<<" locHitPosZ: "<<hitPoint.z()<<G4endl;
  G4double sigma = 9.59*um;
  G4double frac=0.6807;
  G4double diffuseSize = 5*sigma;
  G4double d0=1.406*um;
  G4double base = 0.2988;
  G4int nSector=10;
  G4double secPitch = diffuseSize/(2*nSector+1);
  G4double secEdep;
  G4double eT=0;
  for(G4int iSC=-nSector;iSC<nSector+1;++iSC){
    for(G4int iSR=-nSector;iSR<nSector+1;++iSR){
      G4double dx=secPitch*iSC;
      G4double dy=secPitch*iSR;
      G4double iPosX=hitPoint.x()+dx;
      G4double iPosY=hitPoint.y()+dy;
      //gauss pdf
      G4double pdf = frac*exp(-(pow(dx,2)+pow(dy,2)-d0)/(2*sigma*sigma))+base;

      secEdep = edep*pdf*secPitch*secPitch;
      //G4cout<<"MyMessage::locCol: "<<iSC<<" locRow: "<<iSR<<" Edep: "<<secEdep<<G4endl;
      JadePixId.WhichPixel(iPosX,iPosY);
      PixelIntegration(secEdep,JadePixId,rawHit);
      eT += secEdep;
    }
  }

  //G4cout<<"MyMessage::Total E Collect: "<<eT<<" Total Percent: "<<100*eT/edep<<"%"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void JadePixDigitizer::HitRealizitionEelectrode(JadePixHit* rawHit){

  G4int layerId = rawHit->GetLayerID();
  G4int ladderId = rawHit->GetLadderID();
  G4int chipId = rawHit->GetChipID();
  JadePixIdentifier JadePixId(layerId,ladderId,chipId,-1,-1);

  G4ThreeVector locInPos = rawHit->GetPrePos();
  G4ThreeVector locOutPos = rawHit->GetPostPos();
  if(verboseLevel>0){
    G4cout<<"MyMessage:: locInPos.X: "<<locInPos.x()/cm<<" Y: "<<locInPos.y()/cm<<" Z: "<<locInPos.z()/cm<<G4endl;
    G4cout<<"MyMessage:: locOutPos.X: "<<locOutPos.x()/cm<<" Y: "<<locOutPos.y()/cm<<" Z: "<<locOutPos.z()/cm<<G4endl;
  }

  G4double edep = rawHit->GetEdep();

  G4ThreeVector locMidPos = (locInPos+locOutPos)/2;
  G4int nofCol = JadePixGeo->Layer(0).ColNo();
  G4int nofRow = JadePixGeo->Layer(0).RowNo();
  G4int digiMethod = JadePixGeo->Layer(0).DigiMethod();

  G4ThreeVector locSegPos;
  G4int nSegments = 10; //Step segment
  G4double SegEdep=edep/nSegments; //Uniform distribution of energy in step --> Should change to Landau distribution

  G4double eT;
  G4double eTotal=0;
  G4double ratio;
  G4int nAdjacentPix=5; //Neighbouring pixel hitting

  std::vector<std::vector <G4double>> ePixArray(2*nAdjacentPix+1,std::vector<G4double>(2*nAdjacentPix+1,0));

  for(G4int iSeg=0; iSeg<nSegments; iSeg++){
    locSegPos = (locOutPos-locInPos)/nSegments*(iSeg+0.5)+locInPos; //center of segment
    JadePixId.WhichPixel(locSegPos.x(),locSegPos.y());
    G4int localColID = JadePixId.ColID();
    G4int localRowID = JadePixId.RowID();

    G4double ePix=0;
    eT=0; // Segment tot edep

    for(G4int iC=-nAdjacentPix; iC<nAdjacentPix+1; iC++){
      for(G4int iR=-nAdjacentPix; iR<nAdjacentPix+1; iR++){

        G4int subColID = localColID+iC;
        G4int subRowID = localRowID+iR;
        JadePixIdentifier iJadePixId(layerId,ladderId,chipId,subColID,subRowID);

        if(subColID>-1 && subColID<nofCol && subRowID>-1 && subRowID<nofRow){
          // GaussLorentz diffusion collected in electrode
          if(digiMethod == 0){
            if (verboseLevel>0){ G4cout<<"Gauss lorentz is in use!" << G4endl;}
            ePix = SegEdep*DiffuseGaussLorentzElectrode(iJadePixId,locSegPos);
          }else if(digiMethod == 1){
            if(verboseLevel>0){G4cout<<"Gauss lorentz with four diode is in use!" << G4endl;}
            ePix = SegEdep*OverMOSDiffuseGaussLorentzElectrode(iJadePixId,locSegPos);
          }else if (digiMethod == 2){
            if(verboseLevel>0){G4cout<<"Gauss with center is in use!" << G4endl;}
            //ePix = SegEdep*DiffuseGaussElectrode(iJadePixId,locSegPos);
            ePix = SegEdep*DiffuseGaussElectrodeDivided(iJadePixId,locSegPos);
          }
          if(verboseLevel>0){G4cout<<"SegEdep: "<<SegEdep<<", "<<ePix<<G4endl;}

        }
        ePixArray[nAdjacentPix+iC][nAdjacentPix+iR]=ePix;
        eT += ePix;
      }
    }

    if(verboseLevel>0) {
        std::cout<<"SegEdep: "<<G4BestUnit(SegEdep,"Energy")<<", eSegCollected: "<<G4BestUnit(eT,"Energy")<<endl;
    }

    ratio = cce*SegEdep/eT;
    G4double eTpost = 0;
    for(G4int iC=-nAdjacentPix; iC<nAdjacentPix+1; iC++){
      for(G4int iR=-nAdjacentPix; iR<nAdjacentPix+1; iR++){

        G4int subColID = localColID+iC;
        G4int subRowID = localRowID+iR;
        JadePixIdentifier iJadePixId(layerId,ladderId,chipId,subColID,subRowID);

        ePix = ePixArray[nAdjacentPix+iC][nAdjacentPix+iR]*ratio;
        //cout<<"ePix: "<<ePix<<endl;
        PixelIntegration(ePix,iJadePixId,rawHit);
        eTpost += ePix;
      }
    }
    if(verboseLevel>0) {
        std::cout<<"SegEdep: "<<G4BestUnit(SegEdep,"Energy")<<", eTpost: "<<G4BestUnit(eTpost,"Energy")<<endl;
    }
    eTotal += eTpost;
  }
  if(verboseLevel>0) {
    std::cout<<"edep: "<<G4BestUnit(edep,"Energy")<<", eCollected: "<<G4BestUnit(eTotal,"Energy")<<endl;
  }

}

/*
void JadePixDigitizer::HitRealizitionEelectrodeTest(JadePixHit* rawHit){

  G4int layerId = rawHit->GetLayerID();
  G4int ladderId = rawHit->GetLadderID();
  G4int chipId = rawHit->GetChipID();
  JadePixIdentifier JadePixId(layerId,ladderId,chipId,-1,-1);

  G4ThreeVector locInPos = rawHit->GetPrePos();
  G4ThreeVector locOutPos = rawHit->GetPostPos();
  if(verboseLevel>0){
    G4cout<<"MyMessage:: locInPos.X: "<<locInPos.x()/cm<<" Y: "<<locInPos.y()/cm<<" Z: "<<locInPos.z()/cm<<G4endl;
    G4cout<<"MyMessage:: locOutPos.X: "<<locOutPos.x()/cm<<" Y: "<<locOutPos.y()/cm<<" Z: "<<locOutPos.z()/cm<<G4endl;
  }

  G4double edep = rawHit->GetEdep();

  G4ThreeVector locMidPos = (locInPos+locOutPos)/2;
  G4int nofCol = JadePixGeo->Layer(0).ColNo();
  G4int nofRow = JadePixGeo->Layer(0).RowNo();
  G4int digiMethod = JadePixGeo->Layer(0).DigiMethod();

  G4ThreeVector locSegPos;
  G4int nSegments = 10; //Step segment
  G4double SegEdep=edep/nSegments; //Uniform distribution of energy in step --> Should change to Landau distribution

  G4double eT;
  G4double eTotal=0;
  G4double ratio;
  G4int nAdjacentPix=5; //Neighbouring pixel hitting

  std::vector<std::vector <G4double>> ePixArray(2*nAdjacentPix+1,std::vector<G4double>(2*nAdjacentPix+1,0));

    G4double ePix=0;
    eT=0; // Segment tot edep

  for(G4int iC=-nAdjacentPix; iC<nAdjacentPix+1; iC++){
    for(G4int iR=-nAdjacentPix; iR<nAdjacentPix+1; iR++){
      for(G4int iSeg=0; iSeg<nSegments; iSeg++){
        locSegPos = (locOutPos-locInPos)/nSegments*(iSeg+0.5)+locInPos; //center of segment
        JadePixId.WhichPixel(locSegPos.x(),locSegPos.y());
        G4int localColID = JadePixId.ColID();
        G4int localRowID = JadePixId.RowID();
        G4int subColID = localColID+iC;
        G4int subRowID = localRowID+iR;
        JadePixIdentifier iJadePixId(layerId,ladderId,chipId,subColID,subRowID);

        if(subColID>-1 && subColID<nofCol && subRowID>-1 && subRowID<nofRow){
          // GaussLorentz diffusion collected in electrode
          if(digiMethod == 0){
            if (verboseLevel>0){ G4cout<<"Gauss lorentz is in use!" << G4endl;}
            ePix = SegEdep*DiffuseGaussLorentzElectrode(iJadePixId,locSegPos);
          }else if(digiMethod == 1){
            if(verboseLevel>0){G4cout<<"Gauss lorentz with four diode is in use!" << G4endl;}
            ePix = SegEdep*OverMOSDiffuseGaussLorentzElectrode(iJadePixId,locSegPos);
          }else if (digiMethod == 2){
            if(verboseLevel>0){G4cout<<"Gauss with center is in use!" << G4endl;}
            //ePix = SegEdep*DiffuseGaussElectrode(iJadePixId,locSegPos);
            ePix = SegEdep*DiffuseGaussElectrodeDivided(iJadePixId,locSegPos);
          }
          if(verboseLevel>0){G4cout<<"SegEdep: "<<SegEdep<<", "<<ePix<<G4endl;}

        }
        ePixArray[nAdjacentPix+iC][nAdjacentPix+iR]=ePix;
        eT += ePix;
        }//segment
      }
    }

    if(verboseLevel>0) {
        std::cout<<"SegEdep: "<<G4BestUnit(SegEdep,"Energy")<<", eSegCollected: "<<G4BestUnit(eT,"Energy")<<endl;
    }

    //ratio = cce*SegEdep/eT;
    ratio = 1;
    G4double eTpost = 0;
    for(G4int iC=-nAdjacentPix; iC<nAdjacentPix+1; iC++){
      for(G4int iR=-nAdjacentPix; iR<nAdjacentPix+1; iR++){

        G4int subColID = localColID+iC;
        G4int subRowID = localRowID+iR;
        JadePixIdentifier iJadePixId(layerId,ladderId,chipId,subColID,subRowID);

        ePix = ePixArray[nAdjacentPix+iC][nAdjacentPix+iR]*ratio;
        //cout<<"ePix: "<<ePix<<endl;
        PixelIntegration(ePix,iJadePixId,rawHit);
        //eTpost += ePix;
      }
    }
    if(verboseLevel>0) {
        std::cout<<"SegEdep: "<<G4BestUnit(SegEdep,"Energy")<<", eTpost: "<<G4BestUnit(eTpost,"Energy")<<endl;
    }
    eTotal += eTpost;
  if(verboseLevel>0) {
    std::cout<<"edep: "<<G4BestUnit(edep,"Energy")<<", eCollected: "<<G4BestUnit(eTotal,"Energy")<<endl;
  }

}

*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


double JadePixDigitizer::DiffuseGaussLorentzElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint)
{
  G4double pixPitchX=JadePixGeo->Layer(0).PitchX()*um;
  G4double pixPitchY=JadePixGeo->Layer(0).PitchY()*um;

  //segment impact pixel
  G4double Csigma = 13.2*um;
  G4double CN0=0.458;
  G4double CN1=6.45*um;
  G4double Cd0=-3.98*um;
  G4double Cd1=1.80*um;
  G4double CGamma=3.99*um;

  //Other pixel
  G4double Osigma = 17.5*um;
  G4double ON0=0.117;
  G4double ON1=3.71*um;
  G4double Od0=-1.07*um;
  G4double Od1=-4.64*um;
  G4double OGamma=47.1*um;

  G4double sigma,N0,N1,d0,d1,Gamma;

  G4ThreeVector iPixPos = JadePixId.PixelPos();
  G4double iX=iPixPos.x();
  G4double iY=iPixPos.y();
  G4double dxRaw=hitPoint.x()-iX;
  G4double dyRaw=hitPoint.y()-iY;

  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;

  G4double pixEdgeCut=30*um;
  //G4double nominalPixPitch=22.5*um;
  G4double nominalPixPitch=pixEdgeCut-0.05*(pixPitchX-10*um);
  G4double dx = dxRaw*nominalPixPitch/pixPitchX;
  G4double dy = dyRaw*nominalPixPitch/pixPitchY;

  // The distance between the reconstructed position of the cluster and the diode in collection of pixel in the XY sensor
  G4double d = sqrt(pow(dx,2)+pow(dy,2));


  G4double pdf1,pdf2;
  //if(fabs(dx)<=(pixPitchX/2) && fabs(dy)<=(pixPitchY/2))
  if(fabs(dx)<=pixEdgeCut && fabs(dy)<=pixEdgeCut){
    sigma = Csigma;
    N0    = CN0;
    N1    = CN1;
    d0    = Cd0;
    d1    = Cd1;
    Gamma = CGamma;
  }else{
    sigma = Osigma;
    N0    = ON0;
    N1    = ON1;
    d0    = Od0;
    d1    = Od1;
    Gamma = OGamma;
  }

  // Reference Loic COUSIN
  // Charge Collection
  pdf1=exp(-pow((d-d0),2)/(2*sigma*sigma));
  pdf2=Gamma/(Gamma*Gamma+pow((d-d1),2));

  G4double pdf=N0*pdf1+N1*pdf2;
  G4double factor=1./2;

  return pdf*factor;
}

double JadePixDigitizer::DiffuseGaussElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint)
{
  G4double pixPitchX=JadePixGeo->Layer(0).PitchX()*um;
  G4double pixPitchY=JadePixGeo->Layer(0).PitchY()*um;

  //segment impact pixel
  G4double sigma = 9.59*um;
  G4double N0=0.6807;
  G4double d0=1.406*um;
  G4double base=0.2988;

  //G4double sigma = 10.72*um;
  //G4double N0=0.99;
  //G4double d0=2.94*um;

  G4ThreeVector iPixPos = JadePixId.PixelPos();
  G4double iX=iPixPos.x(); //Diode in the center
  G4double iY=iPixPos.y();
  G4double dxRaw=hitPoint.x()-iX;
  G4double dyRaw=hitPoint.y()-iY;

  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;

  G4double pixEdgeCut=33*um;
  //G4double nominalPixPitch=22.5*um;
  G4double nominalPixPitch=pixEdgeCut-0.05*(pixPitchX-4*um);
  G4double dx = dxRaw*nominalPixPitch/pixPitchX;
  G4double dy = dyRaw*nominalPixPitch/pixPitchY;
  //G4double dx = dxRaw;
  //G4double dy = dyRaw;

  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;

  // The distance between the reconstructed position of the cluster and the diode in collection of pixel in the XY sensor
  G4double d = sqrt(pow(dx,2)+pow(dy,2));

  //cout<<"d-d0: "<<d-d0<<endl;  
  // Charge Collection
  //G4double pdf=N0*exp(-pow((d-d0),2)/(2*sigma*sigma));
  G4double pdf=N0*exp(-pow((d-d0),2)/(2*sigma*sigma))+base;
  //cout<<"pdf: "<<pdf<<endl;

  G4double factor=1.0;

  return pdf*factor;
}

double JadePixDigitizer::DiffuseGaussElectrodeDivided(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint)
{
  G4double pixPitchX=JadePixGeo->Layer(0).PitchX()*um;
  G4double pixPitchY=JadePixGeo->Layer(0).PitchY()*um;
  G4double diodesize=JadePixGeo->Layer(0).DiodeSize()*um;

  //segment impact pixel
  //G4double sigma = 9.59*um;
  //G4double N0=0.6807;
  //G4double d0=1.406*um;
  //G4double base=0.2988;

  G4double mean1 = 1.5*um;
  G4double mean2 = 1.5*um;
  G4double mean3 = 4.*um;
  G4double sigma1 = 2.*um;
  G4double sigma2 = 10.*um;
  G4double sigma3 = 15.*um;
  G4double N0=0.7;
  G4double d0=0*um;
  G4double base=0.3;

  G4ThreeVector iPixPos = JadePixId.PixelPos();
  G4double iX=iPixPos.x(); //Diode in the center
  G4double iY=iPixPos.y();
  G4double dxRaw=hitPoint.x()-iX;
  G4double dyRaw=hitPoint.y()-iY;
 
  const int poly_sides = 8;
  G4double L1 = diodesize/2;
  G4double innerangle = 360/poly_sides;
  G4double alpha = 3.1415926*innerangle/2/180;
  G4double L2 = L1/tan(alpha);
  G4double poly_x[poly_sides] = {iX+L1,iX+L2,iX+L2,iX+L1,-(iX+L1),-(iX+L2),-(iX+L2),-(iX+L1)}; 
  G4double poly_y[poly_sides] = {iY+L2,iY+L1,-(iY+L1),-(iY+L2),-(iY+L2),-(iY+L1),iY+L1,iY+L2};
  G4double x = hitPoint.x();
  G4double y = hitPoint.y();
  G4int ret;
  ret = InOrNot(poly_sides, poly_x, poly_y, x, y);
  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;

  //G4double pixEdgeCut=33*um;
  //G4double nominalPixPitch=22.5*um;
  //G4double nominalPixPitch=pixEdgeCut-0.05*(pixPitchX-4*um);
  //G4double dx = dxRaw*nominalPixPitch/pixPitchX;
  //G4double dy = dyRaw*nominalPixPitch/pixPitchY;
  G4double dx = dxRaw;
  G4double dy = dyRaw;

  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;

  // The distance between the reconstructed position of the cluster and the diode in collection of pixel in the XY sensor
  G4double d = sqrt(pow(dx,2)+pow(dy,2));

  //cout<<"d-d0: "<<d-d0<<endl;  
  // Charge Collection

  G4double pdf;
  if(ret == 1){
    //pdf=N0*exp(-pow((d-d0),mean1)/(2*sigma1*sigma1))+base;
    pdf=1;
  } else if(fabs(dxRaw)<=pixPitchX && fabs(dyRaw)<=pixPitchY){
    pdf=N0*exp(-pow((d-d0),mean2)/(2*sigma2*sigma2))+base;
  }else{ pdf=N0*exp(-pow((d-d0),mean3)/(2*sigma3*sigma3))+base;
  }

  //cout<<"d-d0: "<<d-d0<<endl;
  //cout<<"pdf: "<<pdf<<endl;

  G4double factor=1.0;

  return pdf*factor;
}

double JadePixDigitizer::OverMOSDiffuseGaussLorentzElectrode(JadePixIdentifier& JadePixId, G4ThreeVector hitPoint)
{
  //G4double pixPitchX=JadePixGeo->Layer(0).PitchX()*um;
  //G4double pixPitchY=JadePixGeo->Layer(0).PitchY()*um;

  //center pixel
  G4double Csigma = 42.1*um;
  G4double CN0=17.5;
  G4double CN1=211.8*um;
  G4double Cd0=-230.8*um;
  G4double Cd1=-170.6*um;
  G4double CGamma=23.6*um;

  G4double Osigma = 42.1*um;
  G4double ON0=17.5;
  G4double ON1=211.8*um;
  G4double Od0=-230.8*um;
  G4double Od1=-170.6*um;
  G4double OGamma=23.6*um;

  G4double sigma,N0,N1,d0,d1,Gamma;

  //G4ThreeVector iPixPos = JadePixId.PixelPos();
  std::vector<G4ThreeVector> pixDiode = JadePixId.PixelDiode();
  std::vector<G4ThreeVector>::iterator ipixDiode;
  //G4double iX=iPixPos.x();
  //G4double iY=iPixPos.y();
  //G4double dxRaw=hitPoint.x()-iX;
  //G4double dyRaw=hitPoint.y()-iY;
  G4double dxRaw=0;
  G4double dyRaw=0;

  //cout<<"pixPitchX: "<<pixPitchX<<", pixPitchY: "<<pixPitchY<<endl;
  //cout<<"iX: "<<iX<<", dx: "<<dx<<", iY: "<<iY<<", dy: "<<dy<<endl;
  for(ipixDiode=pixDiode.begin();ipixDiode!=pixDiode.end();++ipixDiode){
    //G4cout<<"Pixel diode vector: "<<*ipixDiode<<G4endl;
    G4double tmp_dxRaw=hitPoint.x()-ipixDiode->x();
    if(tmp_dxRaw<dxRaw){ dxRaw=tmp_dxRaw;}

    G4double tmp_dyRaw=hitPoint.y()-ipixDiode->y();
    if(tmp_dyRaw<dyRaw){ dyRaw=tmp_dyRaw;}
  }
  //G4cout<<"dxRaw: "<<dxRaw<<", dyRaw: "<<dyRaw<<G4endl;

  G4double pixEdgeCut=24*um;
  //G4double nominalPixPitch=22.5*um;
  //G4double nominalPixPitch=pixEdgeCut-0.05*(pixPitchX-10*um);
  //G4double dx = dxRaw*nominalPixPitch/pixPitchX;
  //G4double dy = dyRaw*nominalPixPitch/pixPitchY;
  G4double dx = dxRaw;
  G4double dy = dyRaw;

  // The distance between the reconstructed position of the cluster and the diode in collection of pixel in the XY sensor
  G4double d = sqrt(pow(dx,2)+pow(dy,2));


  G4double pdf1,pdf2;
  //if(fabs(dx)<=(pixPitchX/2) && fabs(dy)<=(pixPitchY/2))
  if(fabs(dx)<=pixEdgeCut && fabs(dy)<=pixEdgeCut){
    sigma = Csigma;
    N0    = CN0;
    N1    = CN1;
    d0    = Cd0;
    d1    = Cd1;
    Gamma = CGamma;
  }else{
    sigma = Osigma;
    N0    = ON0;
    N1    = ON1;
    d0    = Od0;
    d1    = Od1;
    Gamma = OGamma;
  }

  // Reference Loic COUSIN
  // Charge Collection
  pdf1=exp(-pow((d-d0),2)/(2*sigma*sigma));
  pdf2=Gamma/(Gamma*Gamma+pow((d-d1),2));

  G4double pdf=N0*pdf1+N1*pdf2;
  G4double factor=11.33337;

  return pdf*factor;
}

void JadePixDigitizer::PixelIntegration(G4double ePix,JadePixIdentifier& JadePixId, JadePixHit* rawHit){

  G4int colId=JadePixId.ColID();
  G4int rowId=JadePixId.RowID();
  //G4cout<<"MyMessage::colId: "<<colId<<" rowId: "<<rowId<<G4endl;
  G4double globalT = rawHit->GetGlobalT();

  G4int trackId = rawHit->GetTrackID();
  G4int nofCol = JadePixGeo->Layer(0).ColNo();
  G4int nofRow = JadePixGeo->Layer(0).RowNo();

  if(colId>-1 && colId<nofCol && rowId>-1 && rowId<nofRow){
    unsigned int key = JadePixId.PixelID();
    //cout<<"Row: "<<rowId<<" Col: "<<colId<<" Key: "<<key<<endl;

    JadePixHit* _hit = new JadePixHit();
    _hit->SetLayerID(JadePixId.LayerID());
    _hit->SetLadderID(JadePixId.LadderID());
    _hit->SetChipID(JadePixId.ChipID());
    _hit->SetGlobalChipID(JadePixId.GlobalChipID());
    _hit->SetCol(colId);
    _hit->SetRow(rowId);
    //_hit->SetEdep(ePix);
    _hit->AddEdep(ePix);

    _hit->SetTrackID(trackId);
    _hit->SetGlobalT(globalT);

    itRealizedHitMap = realizedHitMap.find(key);
    if(itRealizedHitMap==realizedHitMap.end()){
      realizedHitsCol.push_back(_hit);
      realizedHitMap[key]=G4int(realizedHitsCol.size())-1;
    }
    else{
      G4int pointer=(*itRealizedHitMap).second;
      //cout<<"Key: "<<key<<" pointer: "<<pointer<<endl;
      (realizedHitsCol[pointer])->SetEdep((realizedHitsCol[pointer])->GetEdep()+_hit->GetEdep());
      G4double preGlobalT=(realizedHitsCol[pointer])->GetGlobalT();
      if(globalT<preGlobalT){
        (realizedHitsCol[pointer])->SetGlobalT(globalT);
      }

      delete _hit;

    }
  }

}

void JadePixDigitizer::AddFake(){

  //G4double fakeRate = 10e-4;

  //G4double frPerEvt = fakeRate/600;

  G4int nRow=JadePixGeo->Layer(0).RowNo();
  G4int nCol=JadePixGeo->Layer(0).ColNo();
  //G4int nPix=JadePixGeo->Layer(0).PixNo();
  //  G4int nfakePix = nPix*frPerEvt+G4RandGauss::shoot(0,0.5);
  G4int nfakePix = G4RandGauss::shoot(0,0.7);
  if(nfakePix<0) nfakePix = 0;

  //G4cout<<"MyMessage::nfakePix: "<<nfakePix<<G4endl;

  for(G4int i=0;i<nfakePix;++i){

    G4int rowId=nRow*G4UniformRand();
    G4int colId=nCol*G4UniformRand();

    G4long key=rowId*nCol+colId;
    itDigiMap=digiMap.find(key);
    if(itDigiMap==digiMap.end()){
      G4double fakeE=energyThreshold*G4RandGauss::shoot();
      if(fakeE<energyThreshold) fakeE += 2*(energyThreshold-fakeE);

      JadePixDigi* newDigi = new JadePixDigi();
      newDigi->SetTrackID(-1);
      newDigi->SetLayerID(0);
      newDigi->SetLadderID(0);
      newDigi->SetChipID(0);
      newDigi->SetGlobalChipID(0);
      newDigi->SetRow(rowId);
      newDigi->SetCol(colId);
      newDigi->SetEdep(fakeE);
      newDigi->SetGlobalT(0);
      digisCollection->insert(newDigi);
    }
  }
}

int JadePixDigitizer::GetADC(double eDep){
  int locAdcRange = pow(2,nAdcBit);
  double dE = (adcEnergyRange-energyThreshold)/locAdcRange;
  double eDepEff=eDep-energyThreshold;
  if(eDepEff<0)eDepEff=0;
  int dAdc = int(eDepEff/dE);
  if(dAdc >= locAdcRange) dAdc = locAdcRange-1;
  int adc = int((energyThreshold+dAdc*dE)/ehpEnergy);
  return adc;
}

int JadePixDigitizer::GetVolADC(double eDep){
  int locAdcRange = pow(2,nAdcBit)-1;
  //G4double gain = 0.02*1e-3;
  G4double gain = 0.032*1e-3; // v/e
  //int Vref = 0.5;
  double dE = (adcEnergyRange-energyThreshold)/locAdcRange;
  double eDepEff=eDep-energyThreshold;
  if(eDepEff<0)eDepEff=0;
  int dAdc = int(eDepEff/dE);
  if(dAdc >= locAdcRange) dAdc = locAdcRange-1;
  //cout<<"dAdc: "<<dAdc<<endl;
  //cout<<"adcRange: "<<adcRange<<endl;
  int adc = int(((energyThreshold+dAdc*dE)/ehpEnergy*gain)*locAdcRange);
  //int adc = int(((energyThreshold+dAdc*dE)/ehpEnergy*gain)*adcRange);
  return adc;
}

int JadePixDigitizer::GetTDC(double time){
  int nofBit = 5;
  int tdcRange = pow(2,double(nofBit));
  double range = 5;
  double dT = range/tdcRange;
  int tdc = int(time/dT);
  if(tdc > tdcRange) tdc = tdcRange;
  return tdc;
}

int JadePixDigitizer::InOrNot(int poly_sides, double *poly_x, double *poly_y, double x, double y){
  int i, j;  
  j = poly_sides-1;  
  int res = 0;  
  for (i = 0; i<poly_sides; i++)   
  {  
    if((poly_y[i]<y && poly_y[j]>=y || poly_y[j]<y && poly_y[i]>=y) && (poly_x[i]<=x || poly_x[j]<=x)){  
      res ^= ((poly_x[i] + (y-poly_y[i])/(poly_y[j]-poly_y[i])*(poly_x[j]-poly_x[i])) < x);
    }
    j=i;  
  }
  return res;   
}
