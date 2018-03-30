#ifndef JADE_JADEREAD_HH
#define JADE_JADEREAD_HH

#include "JadeDataFrame.hh"
#include "JadeOption.hh"
#include "JadeSystem.hh"

#include <chrono>
#include <mutex>
#include <queue>
#include <string>

class DLLEXPORT JadeRead {
  public:
  JadeRead(const JadeOption& opt);
  virtual ~JadeRead();
  virtual void Open();
  virtual void Close();
  virtual void Reset();
  virtual std::vector<JadeDataFrameSP> Read(size_t nframe,
      const std::chrono::milliseconds& timeout);
  void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

  private:
  JadeOption m_opt;
  int m_fd;
  std::string m_buf;
  std::vector<std::string> m_path_vec;
};

using JadeReadSP = std::shared_ptr<JadeRead>;

#endif
