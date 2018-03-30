#include "JadeRun.hh"

using namespace std::chrono_literals;

JadeRun::JadeRun(const JadeOption& opt)
    : m_opt(opt)
{
}

JadeRun::~JadeRun()
{
  Reset();
}

void JadeRun::SetReader(JadeReadSP rd)
{
  m_rd = rd;
}

void JadeRun::SetFilter(JadeFilterSP flt)
{
  m_flt = flt;
}

void JadeRun::SetAnalysis(JadeAnalysisSP ana)
{
  m_ana = ana;
}

void JadeRun::Run()
{

  m_ana->Open();

  while (true) {
    size_t nframe_per_read = 10;
    auto v_df = m_rd->Read(nframe_per_read, 10ms);
    size_t nframe_per_read_r = v_df.size();
    if (!v_df.empty()) {
      for (auto& df : v_df) {
        df->Decode();
        m_flt->Filter(df);
        m_ana->Analysis(df);
      }
    }
    if (nframe_per_read_r < nframe_per_read) {
      break;
    }
  }
  m_ana->Close();
}

void JadeRun::Reset()
{
  if (m_rd)
    m_rd->Reset();
  if (m_flt)
    m_flt->Reset();
  if (m_ana)
    m_ana->Reset();
}

void JadeRun::DeviceConnect()
{
  m_rd->Open();
}

void JadeRun::DeviceDisconnect()
{
  m_rd->Close();
}
