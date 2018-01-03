#include "JadePixEvent.h"



JadePixEvent::JadePixEvent(int nChip)
{
  for(int i=0;i<nChip;i++){
    JadePixChip* _chip = new JadePixChip(i);
    m_chipVec.push_back(_chip);
  }
}


JadePixEvent::~JadePixEvent(void)
{
	for(m_itChip=m_chipVec.begin();m_itChip!=m_chipVec.end();++m_itChip){
		delete *m_itChip;						
	}
	m_chipVec.clear();
}


void JadePixEvent::Reset(){
	for(m_itHit=m_hitVec.begin();m_itHit!=m_hitVec.end();++m_itHit){
		delete *m_itHit;	
	}
	m_hitVec.clear();

	for(m_itTrHit=m_trHitVec.begin();m_itTrHit!=m_trHitVec.end();++m_itTrHit){
		delete *m_itTrHit;						
	}
	m_trHitVec.clear();

  m_digiVec.clear();

	for(m_itChip=m_chipVec.begin();m_itChip!=m_chipVec.end();++m_itChip){
		(*m_itChip)->Reset();
	}

  m_trSeed=0;
  m_maxEdep=0;
}

void JadePixEvent::Reconstruct(){
	for(m_itChip=m_chipVec.begin();m_itChip!=m_chipVec.end();++m_itChip){
		(*m_itChip)->FindHit(m_hitVec);						
	}
  
  for(m_itHit=m_hitVec.begin();m_itHit!=m_hitVec.end();++m_itHit){
    (*m_itHit)->Reconstruct();
    for(m_itTrHit=m_trHitVec.begin();m_itTrHit!=m_trHitVec.end();++m_itTrHit){
      if((*m_itTrHit)->GetTrackId()==(*m_itHit)->GetTrackId()) (*m_itHit)->AddTruth(*m_itTrHit);
    }
  }
}

void JadePixEvent::Print(){
	cout<<""<<endl;
	cout<<"********* Number of chips in event "<<m_id<<" :: "<<m_chipVec.size()<<" ***********"<<endl;
  /*
	for(m_itChip=m_chipVec.begin();m_itChip!=m_chipVec.end();++m_itChip){
		(*m_itChip)->Print();
	}
  */

	cout<<"********* Number of hits in event "<<m_id<<" :: "<<m_hitVec.size()<<" ***********"<<endl;
  /*
	for(m_itHit=m_hitVec.begin();m_itHit!=m_hitVec.end();++m_itHit){
		(*m_itHit)->Print();
	}
  */

	cout<<"********* Number of true hits in event "<<m_id<<" :: "<<m_trHitVec.size()<<" ***********"<<endl;
  /*
	for(m_itTrHit=m_trHitVec.begin();m_itTrHit!=m_trHitVec.end();++m_itTrHit){
		(*m_itTrHit)->Print();
	}
  */

}


void JadePixEvent::AddTruth(int trackId,int chipId,double edep,double time,double posX,double posY,double posZ,double enterAngle){
  JadePixHit* _trHit  = new JadePixHit(trackId,chipId,edep,time,posX,posY,posZ,enterAngle);
  m_trHitVec.push_back(_trHit);
}


void JadePixEvent::AddDigi(int trackId,int chipId,int rowId,int colId,double ADC,double TDC){
  JadePixDigi* _digi = new JadePixDigi(trackId,chipId,rowId,colId,ADC,TDC);
  m_digiVec.push_back(_digi);
  (m_chipVec[chipId])->AddDigi(_digi);
	for(m_itTrHit=m_trHitVec.begin();m_itTrHit!=m_trHitVec.end();++m_itTrHit){
		if((*m_itTrHit)->GetTrackId()==trackId) (*m_itTrHit)->AddDigi(_digi);
	}
}
