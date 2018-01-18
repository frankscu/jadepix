//---------------------------------------------------------------------------//
//      BOOST --- BESIII Object_Oriented Simulation Tool                     //
//---------------------------------------------------------------------------//
//Description: Handle database I/O and user interface 
//             for MDC geometry parameters
//Author: Yuan Ye(yuany@mail.ihep.ac.cn)
//Created: 4 Dec, 2003
//Modified:
//Comment: Used in "JadePix" now, should be insert in framwork later
//         The units are "mm" and "rad". 
//         Datum plane is the East Endplane of MDC.
//---------------------------------------------------------------------------//

#include <iostream>
#include <fstream>

#include <CLHEP/Units/PhysicalConstants.h>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"

#include "JadePixHit.hh"
#include "JadePixDigi.hh"
#include "G4ThreeVector.hh"

#include "JadePixWriter.hh"
#include "globals.hh"
#include <math.h>
#include <cstdlib>

JadePixWriter * JadePixWriter::m_JadePixWriter=NULL;

JadePixWriter* JadePixWriter::Instance(){
  if (! m_JadePixWriter) m_JadePixWriter = new JadePixWriter();
  return m_JadePixWriter;
}


void JadePixWriter::WriteEventTag(int id){
  (*m_fout)<<""<<endl;  
  (*m_fout)<<"EventId: "<<id<<endl;    
}

void JadePixWriter::WriteMcTag(int n){
  (*m_fout)<<"McTruth Hit No: "<<n<<endl;    
}

void JadePixWriter::WriteMc(JadePixHit* truthHit){
  G4ThreeVector trPos = truthHit->GetPos();
  (*m_fout)<<truthHit->GetTrackID()<<"\t"<<truthHit->GetGlobalChipID()<<"\t"<<truthHit->GetEdep()<<"\t"<<truthHit->GetGlobalT()<<"\t"<<trPos.x()<<"\t"<<trPos.y()<<"\t"<<trPos.z()<<"\t"<<truthHit->GetEnterAngle()<<endl;
}

void JadePixWriter::WriteMcDebug(JadePixHit* truthHit, G4double edep){
  G4ThreeVector trPos = truthHit->GetPos();
  (*m_fout)<<truthHit->GetTrackID()<<"\t"<<truthHit->GetGlobalChipID()<<"\t"<<edep<<"\t"<<truthHit->GetGlobalT()<<"\t"<<trPos.x()<<"\t"<<trPos.y()<<"\t"<<trPos.z()<<"\t"<<truthHit->GetEnterAngle()<<endl;
}



void JadePixWriter::WriteDigiTag(int n){
  (*m_fout)<<"Digi Hit No: "<<n<<endl;    
}

void JadePixWriter::WriteDigi(JadePixDigi* digiHit){
  (*m_fout)<<digiHit->GetTrackID()<<"\t"<<digiHit->GetGlobalChipID()<<"\t"<<digiHit->GetRow()<<"\t"<<digiHit->GetCol()<<"\t"<<digiHit->GetADC()<<"\t"<<digiHit->GetTDC()<<endl;
}


