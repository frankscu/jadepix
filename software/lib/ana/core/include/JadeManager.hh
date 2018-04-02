#ifndef JADE_JADEMANAGER_HH
#define JADE_JADEMANAGER_HH

#include "JadeSystem.hh"
#include "JadeOption.hh"
#include "JadeDataFrame.hh"
#include "JadeRead.hh"
#include "JadeFilter.hh"
#include "JadeAnalysis.hh"

#include <string>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

class DLLEXPORT JadeManager{
 public:
  JadeManager(const JadeOption &opt);
  virtual ~JadeManager();
  void Reset();

  void SetReader(JadeReadSP rd);
  void SetFilter(JadeFilterSP flt);
  void SetAnalysis(JadeAnalysisSP ana);
  
  void StartDataAnalysing();
  void StopDataAnalysing();
  void DeviceConnect();
  void DeviceDisconnect();
 
 private:
  uint64_t AsyncReading();
  uint64_t AsyncFiltering();
  uint64_t AsyncAnalysising();
  
 private:
  JadeOption m_opt;
  
  JadeReadSP m_rd;
  JadeFilterSP m_flt;
  JadeAnalysisSP m_ana;
  
  bool m_is_running;
  std::future<uint64_t> m_fut_async_rd;
  std::future<uint64_t> m_fut_async_flt;
  std::future<uint64_t> m_fut_async_ana;
  std::mutex m_mx_ev_to_flt;
  std::mutex m_mx_ev_to_ana;
  std::queue<JadeDataFrameSP> m_qu_ev_to_flt;
  std::queue<JadeDataFrameSP> m_qu_ev_to_ana;
  std::condition_variable m_cv_valid_ev_to_flt;
  std::condition_variable m_cv_valid_ev_to_ana;    
};

#endif
