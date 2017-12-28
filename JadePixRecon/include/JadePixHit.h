#pragma once

#ifndef JadePixHit_H
#define JadePixHit_H

#include <vector>

#include <iostream>
#include "Vector/ThreeVector.h"

#include "Units/PhysicalConstants.h"
#include "Units/SystemOfUnits.h"

#include "JadePixDigi.h"
#include "JadePixGeo.h"
typedef CLHEP::Hep3Vector Hep3Vector;

using namespace std;
using namespace CLHEP;

class JadePixHit;
//class MTruthHit;
class JadePixDigi;

class JadePixHit{
  public:
    JadePixHit(int trackId,int chipId, double edep, double time, double posX, double posY, double posZ, double angle):m_trackId(trackId),m_chipId(chipId),m_edep(edep),m_time(time),m_pos(Hep3Vector(posX,posY,posZ)),m_enterAngle(angle){};
    JadePixHit(){};
    ~JadePixHit(void);

    void AddDigi(JadePixDigi *digi){m_digiVec.push_back(digi);};
    int GetNofDigi() const {return m_digiVec.size();};
    void Reconstruct();
    Hep3Vector GetPos() const {return m_pos;};
    int GetId() const {return m_id;};
    void SetId(int id) {m_id=id;};
    void Print();

    double GetADC() const {return m_edep;};
    double GetTDC() const {return m_time;};

    
    void AddTruth(JadePixHit* tr) {m_trHitVec.push_back(tr);};
    int NofTrHit() const {return m_trHitVec.size();};
    JadePixHit* GetTruth(int i) const {return m_trHitVec[i];};
    
    //void SetTruth(JadePixHit* tr) {m_trHit=tr;};
    //JadePixHit* GetTruth const {return m_trHit;};

    void SetChipId(int chip){m_chipId=chip;};
    int GetChipId() const {return m_chipId;};

    void SetTrackId(int track){m_trackId=track;};
    int GetTrackId() const {return m_trackId;};
    
    double GetEnterAngle() const{return m_enterAngle;};
  private:
    int m_trackId;
    int m_chipId;
    double m_edep;
    double m_time;
    Hep3Vector m_pos;
    int m_id;
    vector<JadePixDigi*> m_digiVec;
    vector<JadePixHit*> m_trHitVec;
    //JadePixHit* m_trHit;
    double m_enterAngle;
};

/*
class MTruthHit public: class JadePixHit{
  public:
    MTruthHit(int id, int trackId, int chipId, double edep, double time, double posX, double posY, double posZ, double enterAngle):JadePixHit(id,chipId,edep,time,posX,posY,posZ),m_trackId(trackId),m_enterAngle(enterAngle){};
    ~MTruthHit();

  private:
};
*/

#endif
