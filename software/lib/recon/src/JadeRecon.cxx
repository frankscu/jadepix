#include "JadeRecon.hh"

JadeRecon::JadeRecon():m_evt(NULL),m_jadeIO(NULL)
{
}

JadeRecon::~JadeRecon()
{
    if(m_evt) delete m_evt;
    if(m_jadeIO) delete m_jadeIO;
}

void JadeRecon::runSup(std::string infile, std::string outfile, double thr){

    time_t startSup,endSup;
    time(&startSup);

    JadeSuppress* jadeSup = new JadeSuppress();
    jadeSup->OpenInputFile(infile);
    jadeSup->OpenOutputFile(outfile);

    jadeSup->SetTHR(thr);

    int nChipSup=1;
    JadeEvent* evtSup = new JadeEvent(nChipSup);

    int nEvtSup=0;
    while(jadeSup->ReadEvent(evtSup)>-1){

        jadeSup->WriteEvent(evtSup);

        nEvtSup++;
        if(nEvtSup%1000==0) cout<<nEvtSup<<" events are processed!"<<endl;

        evtSup->Reset();
    }

    delete evtSup;
    delete jadeSup;

    time(&endSup);

    double tSup = difftime(endSup,startSup);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<tSup<<endl;
    cout<<"Total Event: "<<nEvtSup<<"! \t Event Rate: "<<nEvtSup/tSup<<"!"<<endl;
}

void JadeRecon::runRecon(std::string infile, std::string outfile){

    time_t start,end;
    time(&start);

    m_jadeIO = JadeIO::Instance();

    int fdin = m_jadeIO->OpenInputFile(infile);
    if(!fdin){
        std::cerr << "Please check the input raw data file!!!" << std::endl;
        return;
    }

    m_jadeIO->OpenOutputFile(outfile);

    std::string rootfile = outfile.substr(0,outfile.find('.')) + ".root";
    m_jadeIO->OpenROOTFile(rootfile);

    int nChip=1;
    m_evt = new JadeEvent(nChip);

    int nEvt=0;
    while(m_jadeIO->ReadEvent(m_evt)>-1){

        m_evt->Reconstruct();
        m_jadeIO->WriteEvent(m_evt);
        m_jadeIO->WriteROOTFile(m_evt);

        nEvt++;
        if(nEvt%1000==0) cout<<nEvt<<" events are processed!"<<endl;

        m_evt->Reset();
    }
    m_jadeIO->CloseOutputFile();
    m_jadeIO->CloseROOTFile();

    time(&end);
    double t = difftime(end,start);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<t<<endl;
    cout<<"Total Event: "<<nEvt<<"! \t Event Rate: "<<nEvt/t<<"!"<<endl;
}

void JadeRecon::runBin(std::string infile, std::string outfile){

    time_t start,end;
    time(&start);

    JadeIO* jadeIO = JadeIO::Instance();

    jadeIO->OpenInputFile(infile);
    jadeIO->OpenBinaryFile(outfile);


    int nChip=1;
    JadeEvent* evt = new JadeEvent(nChip);

    int nEvt=0;
    while(jadeIO->ReadEvent(evt)>-1){

        jadeIO->WriteBinary(evt);

        nEvt++;
        if(nEvt%1000==0) cout<<nEvt<<" events are processed!"<<endl;

        evt->Reset();
    }


    time(&end);
    double t = difftime(end,start);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<t<<endl;
    cout<<"Total Event: "<<nEvt<<"! \t Event Rate: "<<nEvt/t<<"!"<<endl;
}
