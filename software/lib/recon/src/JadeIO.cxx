#include "JadeIO.hh"

JadeIO::~JadeIO(){
}

JadeIO * JadeIO::m_io=NULL;

JadeIO* JadeIO::Instance(){
    if (! m_io) m_io = new JadeIO();
    return m_io;
}

int JadeIO::OpenInputFile(string filein){
    m_fin = new ifstream(filein.c_str(),ios::in);
    if(!m_fin->is_open()) {
        cerr<<"Error:: Raw data file can NOT be opened!!!"<<endl;
        return 0;
    }
    return 1;
}

int JadeIO::ReadEvent(JadeEvent* evt){

    std::string line;

    int trackId,chipId,rowId,colId;
    double edep,time,posX,posY,posZ,enterAngle,ADC,TDC;

    while(m_fin->good()){
        std::getline(*m_fin,line);
        if(!m_fin->good()){
            return -1;
        }


        if(line.find("EventId") != std::string::npos){
            std::string valStr=line.substr(line.find_last_of(" ")+1);
            m_evtId=atoi(valStr.c_str());
        }

        if(line.find("McTruth") != std::string::npos){
            std::string valStr=line.substr(line.find_last_of(" ")+1);
            m_nTruth=atoi(valStr.c_str());

            for(int i=0;i<m_nTruth;i++){
                (*m_fin)>>trackId>>chipId>>edep>>time>>posX>>posY>>posZ>>enterAngle;
                m_fin->seekg(1,ios::cur);
                evt->AddTruth(trackId,chipId,edep,time,posX,posY,posZ,enterAngle);
            }
        }

        if(line.find("Digi") != std::string::npos){
            //cout<<line<<endl;
            string valStr=line.substr(line.find_last_of(" ")+1);
            //cout<<"DigiNo: "<<valStr<<endl;
            m_nDigi=atoi(valStr.c_str());

            for(int j=0;j<m_nDigi;j++){
                (*m_fin)>>trackId>>chipId>>rowId>>colId>>ADC>>TDC;
                m_fin->seekg(1,ios::cur);
                //cout<<rowId<<" "<<colId<<" "<<ADC<<endl;
                evt->AddDigi(trackId,chipId,rowId,colId,ADC,TDC);
            }
            break;
        }

    }

    return m_evtId;
}


void JadeIO::OpenOutputFile(string fileout){
    m_fout = new ofstream(fileout.c_str(),ios::out);
    if(!m_fout->is_open()){
        cout<<""<<endl;
        cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
        return;
    }
    m_fout->precision(6);
    m_fout->setf(ios::scientific);
    m_fout->setf(ios::right, ios::adjustfield);
    (*m_fout)<<"EvtId\t\tHitId\tChId\tHPosX\t\tHPosY\t\tHADC\tHDigiN\tTPosX\t\tTPosY\t\tTEdep\t\tTDigiN"<<endl;
}

void JadeIO::OpenROOTFile(string fileout){
    m_tfout = TFile::Open(fileout.c_str(), "RECREATE");
    if(!m_tfout->IsOpen()){
        cerr<<"Error:: can NOT open output ROOT file!!!" <<endl;
        return;
    }
    m_tree = new TTree("TDigi", "ROOT tree with a few branches"); 
    m_tree->Branch("m_tpos",&m_tpos,"x/D:y/D:z/D");
    m_tree->Branch("m_tADC",&m_tADC,"m_tADC/I");
    m_tree->Branch("m_trpos",&m_trpos,"x/D:y/D:z/D");
    m_tree->Branch("m_resX", &m_resX,"m_resX/D");
    m_tree->Branch("m_resY", &m_resY,"m_resY/D");
    
    m_ttree = new TTree("TTruth", "ROOT tree with a few branches"); 
    m_ttree->Branch("m_ttrpos",&m_ttrpos,"x/D:y/D:z/D");
    m_ttree->Branch("m_eDep",&m_eDep,"m_eDep/D");
}

int JadeIO::WriteEvent(JadeEvent* evt){
    int nHit=evt->NofHit();
    for(int iHit=0;iHit<nHit;iHit++){
        JadeHit* _hit = evt->GetHit(iHit);
        Hep3Vector pos = _hit->GetPos();
        JadeHit* _trHit = _hit->GetTruth(0);
        Hep3Vector trpos = _trHit->GetPos();

        (*m_fout)<<m_evtId<<"\t\t"<<_hit->GetId()<<"\t"<<_hit->GetChipId()<<"\t"<<pos.x()<<"\t"<<pos.y()<<"\t"<<int(_hit->GetADC())<<"\t"<<_hit->GetNofDigi()<<"\t"<<trpos.x()<<"\t"<<trpos.y()<<"\t"<<_trHit->GetADC()<<"\t"<<_trHit->GetNofDigi()<<endl;
    }
    return 0;
}

int JadeIO::WriteROOTFile(JadeEvent* evt){
    int nHit=evt->NofHit();
    for(int iHit=0;iHit<nHit;iHit++){
        auto _thit = evt->GetHit(iHit);
        m_tpos = _thit->GetPos();
        m_tADC = _thit->GetADC();
        JadeHit* _trhit = _thit->GetTruth(0); 
        m_trpos = _trhit->GetPos();

        m_resX = m_tpos.x()-m_trpos.x(); 
        m_resY = m_tpos.y()-m_trpos.y(); 

        m_tree->Fill();
    } 

    int ntrHit=evt->NofTruth();
    for(int itrHit=0;itrHit<ntrHit;itrHit++){
        auto _ttrHit = evt->GetTruthHit(itrHit);

        m_ttrpos = _ttrHit->GetPos();
        m_eDep = _ttrHit->GetEdep();

        m_ttree->Fill();
    }
    return 0;
}

void JadeIO::OpenOutputFileCluster(string fileout){
    m_foutCluster = new fstream(fileout.c_str(),ios::out | ios::trunc);
    m_foutCluster->precision(6);
    m_foutCluster->setf(ios::scientific);
    m_foutCluster->setf(ios::right, ios::adjustfield);
    if(!m_foutCluster->is_open()){
        cout<<""<<endl;
        cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
        return;
    }
    (*m_foutCluster)<<"EvtId\t\tHitId\tChId\tHPosX\t\tHPosY\t\tHADC\tHDigiN"<<endl;
}

int JadeIO::WriteCluster(JadeEvent* evt){
    int nCluster=evt->NofCluster();
    for(int iCluster=0;iCluster<nCluster;iCluster++){
        JadeCluster* _Cluster = evt->GetCluster(iCluster);
        Hep3Vector pos = _Cluster->GetPos();

        (*m_foutCluster)<<m_evtId<<"\t\t"<<_Cluster->GetId()<<"\t"<<_Cluster->GetChipId()<<"\t"<<pos.x()<<"\t"<<pos.y()<<"\t"<<int(_Cluster->GetADC())<<"\t"<<_Cluster->GetNofDigi()<<endl;
    }
    return 0;
}

void JadeIO::OpenBinaryFile(string fileout){
    m_bfout = new fstream(fileout.c_str(),ios::out | ios::binary);
    if(!m_bfout){
        cout<<""<<endl;
        cout<<"Error::"<<fileout<<" can NOT be opened!!!"<<endl;
    }
    time_t t=time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "**********%Y%m%d%H%M%S**********", localtime(&t));
    (*m_bfout)<<tmp<<"\r\n";
}

void JadeIO::WriteEmptyBinary(JadeEvent*){
    string EventHeader = "\xAA\xAA\xAA\xAA";
    string EventTrailer = "\xF0\xF0\xF0\xF0";
    string RowHeaderFirst = "\x57\x53";
    string RowTrilerFirst = "\x97\x98";

    // Row Head:  0x07F4
    string RowHeaderSecondA[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};
    string RowHeaderSecondB[3]={"\xF4","\xF5","\xF6"};

    // Row Head:  0xE407
    string RowTrilerSecondA[3]={"\xE4","\xE5","\xE6"};
    string RowTrilerSecondB[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};

    (*m_bfout)<<EventHeader;

    for(int row=0; row<48; row++){
        (*m_bfout)<<RowHeaderFirst<<std::hex<<RowHeaderSecondA[row%16]<<std::hex<<RowHeaderSecondB[int(row/16)];
        for(int col=0; col<16; col++){
            (*m_bfout)<<std::hex<<std::setfill('\x00')<<std::setw(2)<<'\x00';
        }
        (*m_bfout)<<RowTrilerFirst<<std::hex<<RowTrilerSecondA[int(row/16)]<<std::hex<<RowTrilerSecondB[row%16];
    }
    (*m_bfout)<<EventTrailer;
}

void JadeIO::WriteBinary(JadeEvent* evt){
    string EventHeader = "\xAA\xAA\xAA\xAA";
    string EventTrailer = "\xF0\xF0\xF0\xF0";
    string RowHeaderFirst = "\x57\x53";
    string RowTrilerFirst = "\x97\x98";

    // Row Head:  0x07F4
    string RowHeaderSecondA[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};
    string RowHeaderSecondB[3]={"\xF4","\xF5","\xF6"};

    // Row Head:  0xE407
    string RowTrilerSecondA[3]={"\xE4","\xE5","\xE6"};
    string RowTrilerSecondB[16]={"\x07","\x17","\x27","\x37","\x47","\x57","\x67","\x77","\x87","\x97","\xA7","\xB7","\xC7","\xD7","\xE7","\xF7"};

    (*m_bfout)<<EventHeader;

    int nDigi=evt->NofDigi();
    for(int row=0; row<48; row++){
        (*m_bfout)<<RowHeaderFirst<<std::hex<<RowHeaderSecondA[row%16]<<std::hex<<RowHeaderSecondB[int(row/16)];
        int counts1=0;
        int counts2=0;
        for(int col=0; col<16; col++){
            bool IsThereData=false;
            for(int iDigi=0;iDigi<nDigi;iDigi++){
                JadeDigi* _digi = evt->GetDigi(iDigi);
                int rowId = _digi->GetRowId();
                int colId = _digi->GetColId();
                int adc = _digi->GetADC();
                if(rowId==row && colId==col){
                    (*m_bfout) << std::setfill('\x00')<<std::setw(2)<<Byte(adc);
                    IsThereData=true;
                    counts1++;
                }
            }

            if(IsThereData==false){
                (*m_bfout)<<std::hex<<std::setfill('\x00')<<std::setw(2)<<'\x00';
                counts2++;
            }
        }
        (*m_bfout)<<RowTrilerFirst<<std::hex<<RowTrilerSecondA[int(row/16)]<<std::hex<<RowTrilerSecondB[row%16];
    }
    (*m_bfout)<<EventTrailer;

}


void JadeIO::CloseOutputFile(){
    m_fout->close();
}

void JadeIO::CloseROOTFile(){
    //m_tfout->Write();
    m_tree->Write();
    m_ttree->Write();
    m_tfout->Close();
}

void JadeIO::CloseBinaryFile(){
    m_bfout->close();
}

void JadeIO::CloseOutputFileCluster(){
    m_foutCluster->close();
}
