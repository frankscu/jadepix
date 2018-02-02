#ifndef JadePixMySession_H
#define JadePixMySession_H 

#include "G4UIsession.hh"
#include <fstream>
#include <iostream>

class JadePixMySession : public G4UIsession
{
    public:
        JadePixMySession();
        ~JadePixMySession();
        virtual G4int ReceiveG4cout(const G4String& coutString);
        virtual G4int ReceiveG4cerr(const G4String& cerrString);

    private:
        std::ofstream logFile, logFile1;

};
#endif
