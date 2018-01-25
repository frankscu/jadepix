#ifndef JadePixSuppress_H
#define JadePixSuppress_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "JadePixEvent.h"
#include "JadePixHit.h"
#include "JadePixDigi.h"


using namespace std;

class JadePixSuppress{
  public:
    JadePixSuppress():m_fin(NULL){
      if(m_suppress){ cout<<"Warning::JadePixSuppress is constructed twice."<<endl;; }
      m_suppress=this;
    };
    ~JadePixSuppress(){if(m_fin) delete m_fin;};

    void OpenInputFile(string filein);
    void CloseInputFile(){m_fin->close();};
    int ReadEvent(JadePixEvent* evt);

    void OpenOutputFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    int WriteEvent(JadePixEvent* evt);

    void WriteEventTag(int id);
    void WriteMc(JadePixHit* truthHit);
    void WriteMcTag(int n);

    void SetTHR(double thr){m_thr = thr;};
    void WriteDigiTag(int n);
    void WriteDigi(JadePixDigi* digi);
    
    static JadePixSuppress* Instance();

  private:

    fstream* m_fin;
    fstream* m_fout;

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    double m_thr;

    static  JadePixSuppress * m_suppress;
};

#endif
