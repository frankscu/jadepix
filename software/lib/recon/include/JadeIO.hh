#ifndef JadeIO_H
#define JadeIO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <exception>

#include "JadeEvent.hh"
#include "JadeHit.hh"

#include "TFile.h"
#include "TTree.h"    

using namespace std;
typedef unsigned char Byte;

class JadeIO{
    public:
        JadeIO():m_fin(NULL){
            if(m_io){ cout<<"Warning::JadeIO is constructed twice."<<endl;; }
            m_io=this;
        };
        ~JadeIO();

        int OpenInputFile(string filein);
        void CloseInputFile(){m_fin->close();};
        int ReadEvent(JadeEvent* evt);

        void OpenOutputFile(string fileout);
        void OpenROOTFile(string fileout);
        void OpenBinaryFile(string fileout);
        void OpenOutputFileCluster(string fileout);
        
        void CloseOutputFile();
        void CloseROOTFile();
        void CloseBinaryFile();
        void CloseOutputFileCluster();
        
        int WriteEvent(JadeEvent* evt);
        int WriteROOTFile(JadeEvent* evt);
        void WriteEmptyBinary(JadeEvent* evt);
        void WriteBinary(JadeEvent* evt);
        int WriteCluster(JadeEvent* evt);

        static JadeIO* Instance();

    private:

        ifstream* m_fin;
        ofstream* m_fout;
        fstream* m_foutCluster;
        fstream* m_bfout;

        TFile*     m_tfout;
        TTree*     m_tree;
        TTree*     m_ttree;
        Hep3Vector m_tpos;
        Hep3Vector m_trpos;       
        Hep3Vector m_ttrpos;       
        Int_t      m_tADC;
        Double_t   m_eDep; 
        Double_t   m_resX;
        Double_t   m_resY;

        int m_evtId;
        int m_nTruth;
        int m_nDigi;

        static  JadeIO * m_io;
};

#endif
