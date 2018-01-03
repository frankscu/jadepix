#include "JadePixDigi.h"
#include <cmath>


JadePixDigi::JadePixDigi(int trackId,int chipId,int rowId,int colId,double ADC, double TDC):m_trackId(trackId),m_globalChipId(chipId),m_rowId(rowId),m_colId(colId),m_adc(ADC),m_tdc(TDC),m_IsInHit(false){
  JadePixGeo* _mGeo = JadePixGeo::GetGeo();
  int nofCol = _mGeo->Layer(0).ColNo();
  int nofRow = _mGeo->Layer(0).RowNo();
  m_id = m_globalChipId*nofCol*nofRow+nofCol*m_rowId+m_colId;
};

JadePixDigi::~JadePixDigi(void)
{
}

bool JadePixDigi::IsAdjacentTo(JadePixDigi* digi){
	int dX=abs(m_colId - digi->GetColId());
	int dY=abs(m_rowId - digi->GetRowId());
	if(dX<2&&dY<2){
		return true;
	}
	else{
		return false;
	}
}

bool JadePixDigi::IsAboveTHR(double threshold){
	if(m_adc>threshold){
		return true;
	}
	else{
		return false;
	}
}
