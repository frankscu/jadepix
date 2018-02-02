#ifndef JadeDigi_H
#define JadeDigi_H

#include "JadeHit.hh"
#include "JadeCluster.hh"
#include "JadeGeo.hh"

class JadeHit;
class JadeCluster;
class JadeDigi
{
    public:
        JadeDigi(void){};
        JadeDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC);
        ~JadeDigi(void);

        inline int GetColId();
        inline int GetRowId();
        inline int Overflow();
        inline int Frame();
        inline int GetId();
        int GetGlobalChipId() const{return m_globalChipId;};
        int GetTrackId() const{return m_trackId;};

        inline JadeHit* Hit();
        inline void Hit(JadeHit *hit);

        inline JadeCluster* Cluster();
        inline void Cluster(JadeCluster *cluster);

        void SetADC(double adc){m_adc=adc;};
        void SetTDC(double tdc){m_tdc=tdc;};

        void SetGlobalChipId(int chipId){m_globalChipId=chipId;};
        void SetTrackId(int trkId){m_trackId=trkId;};

        double GetADC() const {return m_adc;};
        double GetTDC() const {return m_tdc;};

        inline bool IsInHit();
        inline void IsInHit(bool isInHit);

        inline bool IsInCluster();
        inline void IsInCluster(bool isInCluster);

        bool IsAdjacentTo(JadeDigi* digi, int size=2);

        bool IsAboveTHR(double threshold);

    private:
        int m_trackId;
        int m_globalChipId;
        int m_rowId;
        int m_colId;
        int m_overflow;
        int m_frame;
        int m_id;
        double m_adc;
        double m_tdc;
        bool m_IsInHit;
        bool m_IsInCluster;
        JadeHit *m_hit;
        JadeCluster *m_cluster;
};

inline int JadeDigi::GetColId(){
    return m_colId;
}

inline int JadeDigi::GetRowId(){
    return m_rowId;
}

inline int JadeDigi::Overflow(){
    return m_overflow;
}

inline int JadeDigi::Frame(){
    return m_frame;
}

inline int JadeDigi::GetId(){
    return m_id;
}

inline JadeHit* JadeDigi::Hit(){
    return m_hit;
}

inline void JadeDigi::Hit(JadeHit* hit){
    m_hit=hit;
}

inline JadeCluster* JadeDigi::Cluster(){
    return m_cluster;
}

inline void JadeDigi::Cluster(JadeCluster* cluster){
    m_cluster=cluster;
}

inline void  JadeDigi::IsInHit(bool isInHit){
    m_IsInHit=isInHit;
}

inline bool  JadeDigi::IsInHit(){
    return m_IsInHit;
}

inline void  JadeDigi::IsInCluster(bool isInCluster){
    m_IsInCluster=isInCluster;
}

inline bool  JadeDigi::IsInCluster(){
    return m_IsInCluster;
}


#endif
