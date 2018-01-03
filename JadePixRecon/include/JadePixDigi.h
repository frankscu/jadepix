#ifndef JadePixDigi_H
#define JadePixDigi_H

#include "JadePixHit.h"
#include "JadePixGeo.h"

class JadePixHit;
class JadePixDigi
{
public:
	JadePixDigi(void){};
	JadePixDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC);
  ~JadePixDigi(void);

	inline int GetColId();
	inline int GetRowId();
	inline int Overflow();
	inline int Frame();
	inline int GetId();
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

	bool IsAdjacentTo(JadePixDigi* digi);

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
	JadePixHit *m_hit;
};

inline int JadePixDigi::GetColId(){
	return m_colId;
}

inline int JadePixDigi::GetRowId(){
	return m_rowId;
}

inline int JadePixDigi::Overflow(){
	return m_overflow;
}

inline int JadePixDigi::Frame(){
	return m_frame;
}

inline int JadePixDigi::GetId(){
	return m_id;
}

inline JadePixHit* JadePixDigi::Hit(){
	return m_hit;
}

inline void JadePixDigi::Hit(JadePixHit* hit){
	m_hit=hit;
}

inline void  JadePixDigi::IsInHit(bool isInHit){
	m_IsInHit=isInHit;
}

inline bool  JadePixDigi::IsInHit(){
	return m_IsInHit;
}


#endif
