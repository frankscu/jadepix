#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

void chargeDistribution(){

  TH1F* hdx = new TH1F("hdx","hdx",100,-0.05,0.05);
  TH1F* hdy = new TH1F("hdy","hdy",100,-0.05,0.05);

  TH1F* hisADC = new TH1F("hisADC","hisADC",3000,0,28000);
  string line;
  int evtId,hitId,chipId,HitNofDigi,TrNofDigi;
  double posX,posY,hitADC,trPosX,trPosY,edep;
  int nlines=0;
  double THR=0;
  double pedestal=4000;
  double distance[20044],hADC[20044];
  fstream fin("./JadePix.recdat",ios::in);
  std::getline(fin,line);
  cout<<line<<endl;

  while(fin.good()){
    fin>>evtId>>hitId>>chipId>>posX>>posY>>hitADC>>HitNofDigi>>trPosX>>trPosY>>edep>>TrNofDigi;
    fin.seekg(1,ios::cur);
    if(!fin.good()) break;

    //cout<<" posX: "<<posX<<" trPosX: "<<trPosX<<endl;
    //cout<<" posY: "<<posX<<" trPosY: "<<trPosY<<endl;
    double dx = posX-trPosX;
    double dy = posY-trPosY;

    nlines++;

    distance[nlines] = 1e3*sqrt(dx*dx+dy*dy);
    hADC[nlines] = hitADC;
    hdx->Fill(dx);
    hdy->Fill(dy);
    if(hitADC>THR) hisADC->Fill(hitADC+pedestal);
    cout << nlines << "  " << distance[nlines] << "  " << hADC[nlines] << endl;
  }
  fin.close();

  TCanvas* c1 = new TCanvas("c1","c1",1440,720);
  c1->Divide(2,1);
  c1->cd(1);
  hdx->Draw();
  //hdx->Fit("gaus");
  c1->cd(2);
  hdy->Draw();
  //hdy->Fit("gaus");
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  TGraph *tg = new TGraph(double(nlines),distance,hADC);
  tg->Draw("AP");

  TCanvas* c3 = new TCanvas("c3","c3",600,600);
  hisADC->Draw();
  hisADC->SetFillColor(kYellow);
  hisADC->SetLineColor(kYellow);
  TGaxis::SetMaxDigits(3);
  hisADC->DrawNormalized("HIST,same");

  TLegend* lg= new TLegend(0.5,0.7,0.9,0.9);
  lg->AddEntry(hisADC,"Simulation","F2");
  lg->Draw();
}
