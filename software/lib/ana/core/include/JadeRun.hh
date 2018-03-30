#ifndef JADE_JADERUN_HH
#define JADE_JADERUN_HH

#include "JadeSystem.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"
#include "JadeRead.hh"
#include "JadeFilter.hh"
#include "JadeAnalysis.hh"

#include <string>

class DLLEXPORT JadeRun{
 public:
  JadeRun(const JadeOption &opt);
  virtual ~JadeRun();
  void Reset();

  void SetReader(JadeReadSP rd);
  void SetFilter(JadeFilterSP flt);
  void SetAnalysis(JadeAnalysisSP ana);
  
  void Run();
  void DeviceConnect();
  void DeviceDisconnect();
  
 private:
  JadeOption m_opt;
  
  JadeReadSP m_rd;
  JadeFilterSP m_flt;
  JadeAnalysisSP m_ana;

};

#endif
