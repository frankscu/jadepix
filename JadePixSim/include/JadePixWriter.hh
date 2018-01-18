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


#include <string>
#include <vector>

#include "JadePixHit.hh"
#include "JadePixDigi.hh"

using namespace std;


class JadePixWriter{
  public:
    JadePixWriter():m_fout(NULL){
      if(m_JadePixWriter){ G4cout<<"Warning::JadePixWriter is constructed twice."<<endl;; }
      m_JadePixWriter=this;
    };
    ~JadePixWriter(){if(m_fout) delete m_fout;};

    void OpenFile(string fileout){
      m_fout = new fstream(fileout.c_str(),ios::out);
      m_fout->precision(9);
      //m_fout->setf(ios::scientific);
      m_fout->setf(ios::left);
      (*m_fout)<<"TrkId \t ChId \t Edep \t\t time \t\t posX \t\t posY \t\t posZ \t enterAngle"<<endl;
      (*m_fout)<<"TrkId \t ChId \t RowId \t ColId \t ADC \t TDC"<<endl;
      (*m_fout)<<"***********************************************************************"<<endl;
    };

    void WriteEventTag(int id);
    void WriteMcTag(int n);
    void WriteDigiTag(int n);

    void WriteMc(JadePixHit* truthHit);
    void WriteMcDebug(JadePixHit* truthHit, G4double edep);
    void WriteDigi(JadePixDigi* digiHit);

    void CloseFile(){m_fout->close();};

    static JadePixWriter* Instance();

  private:
    
    fstream* m_fout;

    static  JadePixWriter * m_JadePixWriter;

};

#endif
