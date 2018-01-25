#ifndef JadePixMySession_H
#define JadePixMySession_H 1

#include "G4UIsession.hh"
#include "fstream"
#include "iostream"

//#pragma clang diagnostic ignored "-Woverloaded-virtual"

class JadePixMySession : public G4UIsession
{
    public:
        ofstream logFile, logFile1;
        virtual G4int ReceiveG4cout(G4String coutString);
        virtual G4int ReceiveG4cerr(G4String cerrString);
        
        JadePixMySession(){
            logFile.open("results.log", ios::app);
            logFile1.open("errors.log", ios::app);
        }
        
        ~JadePixMySession(){
            logFile.close();
            logFile1.close();
        }
};

G4int JadePixMySession::ReceiveG4cout(G4String coutString)
{
    logFile << coutString << flush;
    return 0;
}

G4int JadePixMySession::ReceiveG4cerr(G4String cerrString)
{
    logFile1 << cerrString << flush;
    return 0;
}
#endif
