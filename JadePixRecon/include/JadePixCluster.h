#ifndef JadePixCluster_H
#define JadePixCluster_H

#include "JadePixDigi.h"
#include "JadePixHit.h"
#include "JadePixGeo.h"

class JadePixDigi;
class JadePixHit;
class JadePixCluster
{
public:
	JadePixCluster(void){};
  JadePixCluster(JadePixDigi* m_digi);
	JadePixCluster(int trackId,int chipId,int rowId,int colId,double ADC, double TDC);
  ~JadePixCluster(void);

	inline int GetColId();
	inline int GetRowId();
	inline int Overflow();
	inline int Frame();
	inline int GetId();
  void SetId(int id) {m_id=id;};
  
  int GetGlobalChipId() const{return m_globalChipId;};
  int GetTrackId() const{return m_trackId;};

	inline JadePixHit* Hit();
	inline void Hit(JadePixHit *hit);

  void SetADC(double adc){m_adc=adc;};
  void SetTDC(double tdc){m_tdc=tdc;};
  
  void SetGlobalChipId(int chipId){m_globalChipId=chipId;};
  void SetTrackId(int trkId){m_trackId=trkId;};

  double GetADC() const {return m_adc;};
  double GetTDC() const {return m_tdc;};

	inline bool IsInHit();
	inline void IsInHit(bool isInHit);

	bool IsAdjacentTo(JadePixCluster* digi);

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
	JadePixHit *m_hit;
};

inline int JadePixCluster::GetColId(){
	return m_colId;
}

inline int JadePixCluster::GetRowId(){
	return m_rowId;
}

inline int JadePixCluster::Overflow(){
	return m_overflow;
}

inline int JadePixCluster::Frame(){
	return m_frame;
}

inline int JadePixCluster::GetId(){
	return m_id;
}

inline JadePixHit* JadePixCluster::Hit(){
	return m_hit;
}

inline void JadePixCluster::Hit(JadePixHit* hit){
	m_hit=hit;
}

inline void  JadePixCluster::IsInHit(bool isInHit){
	m_IsInHit=isInHit;
}

inline bool  JadePixCluster::IsInHit(){
	return m_IsInHit;
}


#endif
