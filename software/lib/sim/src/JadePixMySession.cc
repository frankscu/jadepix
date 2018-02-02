#include "JadePixMySession.hh"

JadePixMySession::JadePixMySession(){
    logFile.open("results.log", std::ios::app);
    logFile1.open("errors.log", std::ios::app);
}

JadePixMySession::~JadePixMySession(){
    logFile.close();
    logFile1.close();
}


G4int JadePixMySession::ReceiveG4cout(const G4String& coutString)
{
    logFile << coutString << std::flush;
    return 0;
}

G4int JadePixMySession::ReceiveG4cerr(const G4String& cerrString)
{
    logFile1 << cerrString << std::flush;
    return 0;
}
