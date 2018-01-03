#ifndef JadePixChip_H
#define JadePixChip_H

#include <iostream>
#include <vector>
#include <map>

#include "JadePixDigi.h"
#include "JadePixCluster.h"
#include "JadePixHit.h"

class JadePixDigi;
class JadePixCluster;
class JadePixHit;

class JadePixChip {
public:
	JadePixChip(int id);
	~JadePixChip(void);

	int GetID() const {return m_id;};
	void SetID(int id){m_id=id;};

	//void FindHit( int (*reconTable)[1152],map<int,JadePixDigi*> &digiVec);
	int FindHit(vector<JadePixHit*> &hitVec);

	void Reset();

  int nDigi() const {return m_digiVec.size();};
  void AddDigi(JadePixDigi* digi){m_digiVec.push_back(digi);};
  JadePixDigi* Digi(int i) const{return m_digiVec[i];};

  void Print();

private:

	vector<JadePixDigi*> m_digiVec;	
	vector<JadePixDigi*>::iterator m_itDigi;	

	vector<JadePixDigi*> m_tempDigiVec;	
	vector<JadePixDigi*>::iterator m_tempItDigi;	

	vector<JadePixCluster*> m_clusterVec;	
	vector<JadePixCluster*>::iterator m_itCluster;	

	int m_id;
};


#endif
