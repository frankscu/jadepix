#ifndef JadePixIO_H
#define JadePixIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "JadePixEvent.h"
#include "JadePixHit.h"
#include <cstdio>

using namespace std;
typedef unsigned char Byte;

class JadePixIO{
  public:
    JadePixIO():m_fin(NULL){
      if(m_io){ cout<<"Warning::JadePixIO is constructed twice."<<endl;; }
      m_io=this;
    };
    ~JadePixIO(){if(m_fin) delete m_fin;};

    void OpenInputFile(string filein);
    void CloseInputFile(){m_fin->close();};
    int ReadEvent(JadePixEvent* evt);

    void OpenOutputFile(string fileout);
    void OpenBinaryFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    void CloseBinaryFile(){m_bfout->close();};
    int WriteEvent(JadePixEvent* evt);
    void WriteEmptyBinary(JadePixEvent* evt);
    void WriteBinary(JadePixEvent* evt);

    static JadePixIO* Instance();

  private:

    fstream* m_fin;
    fstream* m_fout;
    fstream* m_bfout; 

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    static  JadePixIO * m_io;
};

#endif
