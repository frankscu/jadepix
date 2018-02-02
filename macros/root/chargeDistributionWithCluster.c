#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

void chargeDistributionWithCluster(){


  TH1F* hisADC = new TH1F("hisADC","hisADC",5000,0,5000);

  string line;
  int evtId,hitId,chipId,HitNofDigi;
  double posX,posY,hitADC;
  int nlines=0;
  double THR=0;
  double pedestal=0;
  double hADC[500044];
  fstream fin("./JadePix.recdatCluster",ios::in);
  std::getline(fin,line);
  cout<<line<<endl;

  while(fin.good()){
    fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi;
    fin.seekg(1,ios::cur);
    if(!fin.good()) break;

    nlines++;
    if(nlines > 500000) break;

    hADC[nlines] = hitADC;
    if(hitADC>THR) hisADC->Fill(hitADC+pedestal);
  }
  fin.close();

  TCanvas* c3 = new TCanvas("c3","c3",600,600);
  hisADC->Draw();
  hisADC->SetFillColor(kYellow);
  hisADC->SetLineColor(kYellow);

}
