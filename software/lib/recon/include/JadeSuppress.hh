#ifndef JadeSuppress_H
#define JadeSuppress_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "JadeEvent.hh"
#include "JadeHit.hh"
#include "JadeDigi.hh"


using namespace std;

class JadeSuppress{
  public:
    JadeSuppress():m_fin(NULL){};
    ~JadeSuppress(){if(m_fin) delete m_fin;};

    void OpenInputFile(string filein);
    void CloseInputFile(){m_fin->close();};
    int ReadEvent(JadeEvent* evt);

    void OpenOutputFile(string fileout);
    void CloseOutputFile(){m_fout->close();};
    int WriteEvent(JadeEvent* evt);

    void WriteEventTag(int id);
    void WriteMc(JadeHit* truthHit);
    void WriteMcTag(int n);

    void SetTHR(double thr){m_thr = thr;};
    void WriteDigiTag(int n);
    void WriteDigi(JadeDigi* digi);

  private:

    fstream* m_fin;
    fstream* m_fout;

    int m_evtId;
    int m_nTruth;
    int m_nDigi;

    double m_thr;

};

#endif
