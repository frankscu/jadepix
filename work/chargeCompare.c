#include<iostream>
using namespace std;

void chargeCompare(){

  TString rootposition, epsname[3];  
  rootposition  = "./Round.root";
  epsname[0] = "dxdy.eps" ;
  epsname[1] = "ADCdistance.eps" ;
  epsname[2] = "spectra.eps" ; 
           
  TFile *f;
  TTree *t;
  
  Double_t distance,hADC;
  Double_t dx,dy,ADCValue;
     
  f = new TFile(rootposition);
  t = (TTree *)f->Get("t");
  
  t->SetBranchAddress("dx",   &dx);
  t->SetBranchAddress("dy",   &dy);
  t->SetBranchAddress("distance",   &distance);
  t->SetBranchAddress("hADC",   &hADC);
  t->SetBranchAddress("ADCValue",   &ADCValue);
 
  //long int TotNum = t->GetEntries();
  TCanvas *c1 = new TCanvas("c1", "c1", 1440,720);
  c1->Divide(2,1); 
  TH1F* hdx = new TH1F("hdx","hdx",100,-0.4,0); 
  TH1F* hdy = new TH1F("hdy","hdy",100,-0.05,0.05);
  TCanvas *c2 = new TCanvas("c2","c2",600,800);
  c2->Divide(1,1);
  TH2F* hdisADC = new TH2F("hdisADC","hdisADC",10000,0,400,10000,0,5000);
  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  c3->Divide(1,1);
  TH1F* hisADC = new TH1F("hisADC","hisADC",100000,0,5000);
  
  for(int i=0; i<t->GetEntries(); i++)
  {
    t->GetEntry(i);
    //cout<<" "<<dx<<" "<<dy<<" "<<ADCValue<<" "<<distance<<" "<<hADC<<endl;
    hdx->Fill(dx);
    hdy->Fill(dy);
    hisADC->Fill(ADCValue);
    hdisADC->Fill(distance,hADC);
  }
  
  c1->cd(1);
  hdx->Draw();
  c1->cd(2);
  hdy->Draw();
  
  c2->cd(1);
  hdisADC->Draw();
  
  c3->cd(1);
  hisADC->Draw();
  hisADC->SetFillColor(kYellow);
  hisADC->SetLineColor(kYellow);
  //hisADC->DrawNormalized("HIST,smae");
  TLegend* lg = new TLegend(0.5,0.7,0.9,0.9);
  lg->AddEntry(hisADC,"Simulation","F2");
  lg->Draw("same");
}
