#include "JadeDigi.hh"
#include <cmath>


JadeDigi::JadeDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC):m_trackId(trackId),m_globalChipId(chipId),m_rowId(rowId),m_colId(colId),m_adc(ADC),m_tdc(TDC),m_IsInHit(false),m_IsInCluster(false){
  JadeGeo* _mGeo = JadeGeo::GetGeo();
  int nofCol = _mGeo->Layer(0).ColNo();
  int nofRow = _mGeo->Layer(0).RowNo();
  m_id = m_globalChipId*nofCol*nofRow+nofCol*m_rowId+m_colId;
}

JadeDigi::~JadeDigi(void)
{;}

bool JadeDigi::IsAdjacentTo(JadeDigi* digi, int size){
	int dX=abs(m_colId - digi->GetColId());
	int dY=abs(m_rowId - digi->GetRowId());
	if(dX<size&&dY<size){
		return true;
	}
	else{
		return false;
	}
}


bool JadeDigi::IsAboveTHR(double threshold){
	if(m_adc>threshold){
		return true;
	}
	else{
		return false;
	}
}
