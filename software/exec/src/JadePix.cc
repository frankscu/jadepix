#include "JadeRecon.hh"
#include "JadePixSim.hh"
#include <string>
#include <iostream>

int main(int argc, char **argv){

    auto pSim = new JadePixSim();
    pSim->runSim(argc, argv);
    
    std::string recon_file = "JadePix.recdat";
    std::string raw_file;
    std::string::size_type pos;

    if(argc > 2){
        raw_file = argv[2];
    }else{
        raw_file = "JadePix.rawdat";
    }

    pos = raw_file.find('.');
    recon_file = raw_file.substr(0,pos) + ".recdat";

    auto pRecon = new JadeRecon();

    pRecon->runRecon(raw_file,recon_file);

    delete pSim;
    delete pRecon;

    return 0;
}
