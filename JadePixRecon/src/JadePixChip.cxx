#include "JadePixChip.h"


JadePixChip::JadePixChip(int id):m_id(id)
{
}

JadePixChip::~JadePixChip(void)
{
	for(m_itDigi=m_digiVec.begin();m_itDigi!=m_digiVec.end();++m_itDigi){
		delete *m_itDigi;						
	}
	m_digiVec.clear();
}


void JadePixChip::Reset(){
	for(m_itDigi=m_digiVec.begin();m_itDigi!=m_digiVec.end();++m_itDigi){
		delete *m_itDigi;	
	}
	m_digiVec.clear();
}


void JadePixChip::Print(){
	cout<<""<<endl;
	cout<<"********* Number of digis in frame "<<m_id<<" :: "<<m_digiVec.size()<<" ***********"<<endl;
  /*
	for(m_itDigi=m_digiVec.begin();m_itDigi!=m_digiVec.end();++m_itDigi){
		(*m_itDigi)->Print();
	}
  */

}


int JadePixChip::FindHit(vector<JadePixHit*> &hitVec){

  if(m_digiVec.size()<1) return 0;

  JadePixHit* _hit;	
  vector<JadePixDigi*>::iterator itDigi;
  vector<JadePixDigi*>::iterator _secondItDigi;

  vector <JadePixDigi*> _inHitVec;
  vector<JadePixDigi*>::iterator _itDigiInHit;

  for(itDigi=m_digiVec.begin();itDigi!=m_digiVec.end();++itDigi){
    //		cout<<itDigi->first<<endl;

    JadePixDigi* _digi=(*itDigi);		

    if(!_digi->IsInHit()) m_tempDigiVec.push_back(_digi);
    //		bool adjacentInHit=false;

    _secondItDigi = itDigi;
    ++_secondItDigi;
    for(; _secondItDigi!=m_digiVec.end(); ++_secondItDigi){	

      if(_digi->IsAdjacentTo(*_secondItDigi)){
        if(!_digi->IsInHit()){	
          if(!(*_secondItDigi)->IsInHit()){
            m_tempDigiVec.push_back(*_secondItDigi);
          }
          else{
            _hit=(*_secondItDigi)->Hit();
            for(m_tempItDigi=m_tempDigiVec.begin();m_tempItDigi!=m_tempDigiVec.end();++m_tempItDigi){
              _hit->AddDigi(*m_tempItDigi);
              (*m_tempItDigi)->IsInHit(true);
              (*m_tempItDigi)->Hit(_hit);
            }

            m_tempDigiVec.clear();

          }
        }
        else{
          _hit=_digi->Hit();
          if(!(*_secondItDigi)->IsInHit()){
            _hit->AddDigi(*_secondItDigi);
            (*_secondItDigi)->IsInHit(true);
            (*_secondItDigi)->Hit(_hit);
          }
          //
          else{
            if(_hit->GetId() != (*_secondItDigi)->Hit()->GetId()){							
              cout<<""<<endl;
              cout<<"************************************************************"<<endl;
              cout<<"Warnning in FindHit: One digi is adjacent to different hits!!!"<<endl;
              cout<<""<<endl;
              cout<<"Digis with wrong:"<<endl;

              cout<<"Row: "<<_digi->GetRowId()<<"   Col: "<<_digi->GetColId()<<"   ID: "<<_digi->GetId()<<endl;
              cout<<"Row: "<<(*_secondItDigi)->GetRowId()<<"   Col: "<<(*_secondItDigi)->GetColId()<<"   ID: "<<(*_secondItDigi)->GetId()<<endl;

              //_hit->Print();
              //(*_secondItDigi)->Hit()->Print();
              cout<<"***********************************************************"<<endl;
              cout<<""<<endl;
              return 1;
            }						
          }
          //
        }
      }

    }

    if(!_digi->IsInHit()){
      _hit = new JadePixHit();
      _hit->SetId(hitVec.size());
      hitVec.push_back(_hit);
      for(m_tempItDigi=m_tempDigiVec.begin();m_tempItDigi!=m_tempDigiVec.end();++m_tempItDigi){
        _hit->AddDigi(*m_tempItDigi);
        (*m_tempItDigi)->IsInHit(true);
        (*m_tempItDigi)->Hit(_hit);
      }

      m_tempDigiVec.clear();
    }

  }	
  return 0;
}

