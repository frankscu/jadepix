#ifndef JadeRecon_H
#define JadeRecon_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <time.h>

#include "JadeIO.hh"
#include "JadeEvent.hh"
#include "JadeDigi.hh"
#include "JadeHit.hh"
#include "JadeChip.hh"
#include "JadeCluster.hh"
#include "JadeSuppress.hh"


class JadeIO;
class JadeEvent;
class JadeDigi;
class JadeHit;
class JadeChip;
class JadeCluster;
class JadeSuppress;

class  JadeRecon {
    public:
        JadeRecon();
        ~JadeRecon();

        void runSup(std::string infile, std::string outfile, double thr);
        void runRecon(std::string infile, std::string outfile);
        void runBin(std::string infile, std::string outfile);

    private:
        JadeEvent* m_evt;
        JadeIO* m_jadeIO;
};

#endif //JadeRecon_H

