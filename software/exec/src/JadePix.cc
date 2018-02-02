#include "JadeRecon.hh"
#include "JadePixSim.hh"

int main(int argc, char **argv){

    auto pSim = new JadePixSim(argc, argv);

    //auto pRecon = new JadeRecon();
    //pRecon->runRecon("/home/chenlj/jadepix/work/JadePix.rawdat","/home/chenlj/jadepix/work/JadePix.recdat");

    //delete pRecon;
    delete pSim; 
    
    return 0;
}
