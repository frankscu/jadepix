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

#ifndef JadePixWriter_H
#define JadePixWriter_H 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include <CLHEP/Units/PhysicalConstants.h>
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"

#include "JadePixHit.hh"
#include "JadePixDigi.hh"

using namespace std;

class JadePixWriter{
  public:
    JadePixWriter();
    ~JadePixWriter();

    int OpenFile(string fileout);

    void WriteEventTag(int id);
    void WriteMcTag(int n);
    void WriteDigiTag(int n);

    void WriteMc(JadePixHit* truthHit, G4double totalEdep);
    void WriteDigi(JadePixDigi* digiHit);

    void CloseFile(){m_fout->close();};

    static JadePixWriter* Instance();

  private:
    
    ofstream* m_fout;

    static JadePixWriter* m_JadePixWriter;
};

#endif
