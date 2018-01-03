// JadePixRecon.cpp
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <time.h>

#include "JadePixIO.h"
#include "JadePixEvent.h"
#include "JadePixDigi.h"
#include "JadePixHit.h"
#include "JadePixChip.h"
#include "JadePixSuppress.h"

using namespace std;


int main(int argc, char* argv[])
{
  if(argc < 4) {
    cerr << "Usage: \n"
         << "   JadePixRecon -s: Suppress \n" 
         << "   JadePixRecon -r: Reconstruct" << endl;
    return -1;
  }

  if(strcmp(argv[1],"-s")==0){

    time_t startSup,endSup;
    time(&startSup);

    string infile = argv[2];
    string outfile = argv[3];

    JadePixSuppress* jadepixSup = JadePixSuppress::Instance();
    jadepixSup->OpenInputFile(infile);
    jadepixSup->OpenOutputFile(outfile);

    double thr=0;
    if(argc==6){
      if (strcmp(argv[4],"-THR")==0){
        thr=atof(argv[5]);
        jadepixSup->SetTHR(thr);
      }else{
        jadepixSup->SetTHR(thr);
      }
    }else{
      jadepixSup->SetTHR(50);
    }

    int nChipSup=1;
    JadePixEvent* evtSup = new JadePixEvent(nChipSup);

    int nEvtSup=0;
    while(jadepixSup->ReadEvent(evtSup)>-1){

      jadepixSup->WriteEvent(evtSup);

      nEvtSup++;
      if(nEvtSup%1000==0) cout<<nEvtSup<<" events are processed!"<<endl;

      evtSup->Reset();
    }

    delete evtSup;
    delete jadepixSup;

    time(&endSup);

    double tSup = difftime(endSup,startSup);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<tSup<<endl;
    cout<<"Total Event: "<<nEvtSup<<"! \t Event Rate: "<<nEvtSup/tSup<<"!"<<endl;

  }else if(strcmp(argv[1],"-r")==0){

    time_t start,end;
    time(&start);

    string infile = argv[2];
    string outfile = argv[3];

    JadePixIO* jadepixIO = JadePixIO::Instance();

    jadepixIO->OpenInputFile(infile);
    jadepixIO->OpenOutputFile(outfile);

    if(argc>4) {
      cerr << "Usage: " << endl;
      return -1;
    }

    int nChip=1;
    JadePixEvent* evt = new JadePixEvent(nChip);

    int nEvt=0;
    while(jadepixIO->ReadEvent(evt)>-1){

      evt->Reconstruct();
      jadepixIO->WriteEvent(evt);

      nEvt++;
      if(nEvt%1000==0) cout<<nEvt<<" events are processed!"<<endl;

      evt->Reset();
    }

    delete evt;
    delete jadepixIO;

    time(&end);
    double t = difftime(end,start);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<t<<endl;
    cout<<"Total Event: "<<nEvt<<"! \t Event Rate: "<<nEvt/t<<"!"<<endl;


    //getchar();
  }else if(strcmp(argv[1],"-b")==0){
    time_t start,end;
    time(&start);

    string infile = argv[2];
    string outfile = argv[3];

    JadePixIO* jadepixIO = JadePixIO::Instance();

    jadepixIO->OpenInputFile(infile);
    jadepixIO->OpenBinaryFile(outfile);

    if(argc>4) {
      cerr << "Usage: " << endl;
      return -1;
    }

    int nChip=1;
    JadePixEvent* evt = new JadePixEvent(nChip);

    int nEvt=0;
    while(jadepixIO->ReadEvent(evt)>-1){

      jadepixIO->WriteBinary(evt);

      nEvt++;
      if(nEvt%1000==0) cout<<nEvt<<" events are processed!"<<endl;

      evt->Reset();
    }

    delete evt;
    delete jadepixIO;

    time(&end);
    double t = difftime(end,start);
    cout<<"*****************************************************************"<<endl;
    cout<<"Time Cost: "<<t<<endl;
    cout<<"Total Event: "<<nEvt<<"! \t Event Rate: "<<nEvt/t<<"!"<<endl;

  }
  return 0;
}
