#ifndef JadeChip_H
#define JadeChip_H

#include <iostream>
#include <vector>
#include <map>

#include "JadeDigi.hh"
#include "JadeCluster.hh"
#include "JadeHit.hh"

class JadeDigi;
class JadeCluster;
class JadeHit;

class JadeChip {
public:
	JadeChip(int id);
	~JadeChip(void);

	int GetID() const {return m_id;};
	void SetID(int id){m_id=id;};

	//void FindHit( int (*reconTable)[1152],map<int,JadeDigi*> &digiVec);
	int FindHit(vector<JadeHit*> &hitVec);
	
    int FindCluster(vector<JadeCluster*> &clusterVec, int size);

	void Reset();

  int nDigi() const {return m_digiVec.size();};
  void AddDigi(JadeDigi* digi){m_digiVec.push_back(digi);};
  JadeDigi* Digi(int i) const{return m_digiVec[i];};

  void Print();

private:

	vector<JadeDigi*> m_digiVec;	
	vector<JadeDigi*>::iterator m_itDigi;	

	vector<JadeDigi*> m_tempDigiVec;	
	vector<JadeDigi*>::iterator m_tempItDigi;	

	vector<JadeCluster*> m_clusterVec;	
	vector<JadeCluster*>::iterator m_itCluster;	

	int m_id;
};


#endif
