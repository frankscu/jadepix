#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

void chargeDistribution(){

  string line;
  int evtId,hitId,chipId,HitNofDigi,TrNofDigi;
  double posX,posY,hitADC,trPosX,trPosY,edep;
  int nlines=0;
  double THR=0;
  double pedestal=0;
  TString recfile;
  double_t distance,hADC;
  double_t dx,dy,ADCValue;
  TFile *f = new TFile("Round.root","recreate");
  TTree *t = new TTree("t","tree 1");
  t->Branch("dx",&dx,"dx/D");
  t->Branch("dy",&dy,"dy/D");
  t->Branch("distance",&distance,"distance/D");
  t->Branch("hADC",&hADC,"hADC/D");
  t->Branch("ADCValue",&ADCValue,"ADCValue/D");
  for (int i=0; i<50;i++){
  stringstream ss;
  ss << i;
  recfile = "JadePix_" + ss.str() + ".recdat";
  fstream fin(recfile,ios::in);
  std::getline(fin,line);
  cout<<line<<endl;
  
  while(fin.good()){
    fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi>>trPosX>>trPosY>>edep>>TrNofDigi;
    fin.seekg(1,ios::cur);
    if(!fin.good()) break;

    //cout<<" posX: "<<posX<<" trPosX: "<<trPosX<<endl;
    //cout<<" posY: "<<posX<<" trPosY: "<<trPosY<<endl;
    dx = posX-trPosX;
    dy = posY-trPosY;
    distance = 1e3*sqrt(dx*dx+dy*dy);
    hADC = hitADC;
    if(hitADC>THR) ADCValue = hitADC+pedestal;
    cout << nlines << "  " << distance << "  " << hADC << endl;
    t->Fill();
    nlines++;
  }
  fin.close();
  }
  t->Write();
}
