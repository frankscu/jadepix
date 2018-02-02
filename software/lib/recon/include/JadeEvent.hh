#ifndef JadeEvent_H
#define JadeEvent_H

#include <iostream>
#include <vector>
#include <map>

#include "JadeChip.hh"
#include "JadeCluster.hh"
#include "JadeHit.hh"

class JadeChip;
class JadeHit;
class JadeDigi;
class JadeEvent{
    public:
        JadeEvent(int nChip);
        ~JadeEvent(void);

        void Reset();

        void Reconstruct();

        int GetID() const {return m_id;};
        void SetID(int id){m_id=id;};

        int NofHit(){return m_hitVec.size();}
        JadeHit* GetHit(int i) const {return m_hitVec[i];};

        int NofCluster(){return m_clusterVec.size();}
        JadeCluster* GetCluster(int i) const {return m_clusterVec[i];};

        int NofTruth(){return m_trHitVec.size();}
        JadeHit* GetTruthHit(int i) const {return m_trHitVec[i];};

        int NofDigi(){return m_digiVec.size();}
        JadeDigi* GetDigi(int i) const {return m_digiVec[i];};

        int NofChip(){return m_chipVec.size();}
        JadeChip* GetChip(int i) const {return m_chipVec[i];};

        void AddTruth(int trackId,int chipId,double edep,double time,double posX,double posY,double posZ,double enterAngle);
        void AddDigi(int trackId,int chipId,int rowId,int colId,double ADC,double TDC);

        void Print();

    private:

        vector<JadeHit*> m_hitVec;
        vector<JadeHit*>::iterator m_itHit;

        vector<JadeCluster*> m_clusterVec;
        vector<JadeCluster*>::iterator m_itCluster;

        vector<JadeHit*> m_trHitVec;
        vector<JadeHit*>::iterator m_itTrHit;
        int m_trSeed;
        double m_maxEdep;

        vector<JadeChip*> m_chipVec;
        vector<JadeChip*>::iterator m_itChip;

        vector<JadeDigi*> m_digiVec;
        vector<JadeDigi*>::iterator m_itDigi;

        int m_id;
};


#endif
