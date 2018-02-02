#ifndef JadeIO_H
#define JadeIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "JadeEvent.hh"
#include "JadeHit.hh"
#include <cstdio>

using namespace std;
typedef unsigned char Byte;

class JadeIO{
    public:
        JadeIO():m_fin(NULL){
            if(m_io){ cout<<"Warning::JadeIO is constructed twice."<<endl;; }
            m_io=this;
        };
        ~JadeIO();

        void OpenInputFile(string filein);
        void CloseInputFile(){m_fin->close();};
        int ReadEvent(JadeEvent* evt);

        void OpenOutputFile(string fileout);
        void OpenBinaryFile(string fileout);
        void CloseOutputFile(){m_fout->close();};
        void CloseBinaryFile(){m_bfout->close();};
        int WriteEvent(JadeEvent* evt);
        void WriteEmptyBinary(JadeEvent* evt);
        void WriteBinary(JadeEvent* evt);

        void OpenOutputFileCluster(string fileout);
        int WriteCluster(JadeEvent* evt);


        static JadeIO* Instance();

    private:

        fstream* m_fin;
        fstream* m_fout;
        fstream* m_foutCluster;
        fstream* m_bfout;

        int m_evtId;
        int m_nTruth;
        int m_nDigi;

        static  JadeIO * m_io;
};

#endif
