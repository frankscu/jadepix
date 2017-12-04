#ifndef JadepixFrame_H
#define JadepixFrame_H

// c++
#include <iostream>

// Jadepix
#include "JadepixDigi.h"

class JadepixFrame{
  public:
    JadepixFrame(){};
    JadepixFrame(int frameId):jadepix_frameId(frameId){};
    ~JadepixFrame(){};

    void SetFrameId(int frameId) {jadepix_frameId = frameId;};
    int GetFrameId() const {return jadepix_frameId;};

    int NoOfDigi(){return jadepix_digiVec.size();};
    JadepixDigi* GetDigi(int digiId) const {return jadepix_digiVec[digiId];};

    void AddDigi(JadepixDigi* digi){jadepix_digiVec.push_back(digi);};

  private:
    int jadepix_frameId;
    std::vector<JadepixDigi*>jadepix_digiVec; 
    std::vector<JadepixDigi*>::iterator jadepix_itdigi; 
};

#endif //JadepixFrame_H
