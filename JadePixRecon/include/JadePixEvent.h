#ifndef JadePixEvent_H
#define JadePixEvent_H

#include <iostream>
#include <vector>
#include <map>

#include "JadePixChip.h"
#include "JadePixCluster.h"
#include "JadePixHit.h"

class JadePixChip;
class JadePixHit;
class JadePixDigi;
class JadePixEvent{
public:
	JadePixEvent(int nChip);
	~JadePixEvent(void);

	void Reset();
	
  void Reconstruct();

	int GetID() const {return m_id;};
	void SetID(int id){m_id=id;};

	int NofHit(){return m_hitVec.size();}
  JadePixHit* GetHit(int i) const {return m_hitVec[i];};

	int NofTruth(){return m_trHitVec.size();}
  JadePixHit* GetTruthHit(int i) const {return m_trHitVec[i];};

	int NofDigi(){return m_digiVec.size();}
  JadePixDigi* GetDigi(int i) const {return m_digiVec[i];};

	int NofChip(){return m_chipVec.size();}
  JadePixChip* GetChip(int i) const {return m_chipVec[i];};

  void AddTruth(int trackId,int chipId,double edep,double time,double posX,double posY,double posZ,double enterAngle);
  void AddDigi(int trackId,int chipId,int rowId,int colId,double ADC,double TDC);

	void Print();

private:

	vector<JadePixHit*> m_hitVec;	
	vector<JadePixHit*>::iterator m_itHit;

	vector<JadePixHit*> m_trHitVec;	
	vector<JadePixHit*>::iterator m_itTrHit;
  int m_trSeed;
  double m_maxEdep;

	vector<JadePixChip*> m_chipVec;	
	vector<JadePixChip*>::iterator m_itChip;	

	vector<JadePixDigi*> m_digiVec;	
	vector<JadePixDigi*>::iterator m_itDigi;	

	int m_id;
};


#endif
