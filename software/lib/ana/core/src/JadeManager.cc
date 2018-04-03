#include "JadeManager.hh"

using namespace std::chrono_literals;

JadeManager::JadeManager(const JadeOption& opt)
    : m_is_running(false)
    , m_opt(opt)
{
}

JadeManager::~JadeManager()
{
  Reset();
}

void JadeManager::SetReader(JadeReadSP rd)
{
  m_rd = rd;
}

void JadeManager::SetFilter(JadeFilterSP flt)
{
  m_flt = flt;
}

void JadeManager::SetAnalysis(JadeAnalysisSP ana)
{
  m_ana = ana;
}

uint64_t JadeManager::AsyncReading()
{
  auto tp_start = std::chrono::system_clock::now();
  auto tp_print_prev = std::chrono::system_clock::now();
  uint64_t ndf_print_next = 10000;
  uint64_t ndf_print_prev = 0;
  uint64_t n_df = 0;
  while (m_is_running) {
    size_t nframe_per_read = 10;
    auto v_df = m_rd->Read(nframe_per_read, 300ms);
    size_t nframe_per_read_r = v_df.size();
    //if (!n_df && !v_df.empty()) {
      std::unique_lock<std::mutex> lk_out(m_mx_ev_to_flt);
      for (auto&& df : v_df) {
        m_qu_ev_to_flt.push(df);
        n_df++;
      }
      lk_out.unlock();
      m_cv_valid_ev_to_flt.notify_all();
    //}
    if (n_df >= ndf_print_next) {
      auto tp_now = std::chrono::system_clock::now();
      auto dur = tp_now - tp_start;
      double dur_sec_c = dur.count() * decltype(dur)::period::num * 1.0 / decltype(dur)::period::den;
      double av_hz = n_df / dur_sec_c;
      dur = tp_now - tp_print_prev;
      dur_sec_c = dur.count() * decltype(dur)::period::num * 1.0 / decltype(dur)::period::den;
      double cr_hz = (n_df - ndf_print_prev) / dur_sec_c;
      std::cout << "JadeManager:: average data rate<" << av_hz << "> current data rate<" << cr_hz << ">" << std::endl;
      ndf_print_next += 10000;
      ndf_print_prev = n_df;
      tp_print_prev = tp_now;
    }
    if (nframe_per_read_r < nframe_per_read) {
      break;
    }
  }
  return n_df;
}

uint64_t JadeManager::AsyncFiltering()
{
  uint64_t n_df = 0;
  while (m_is_running) {
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_flt);
    while (m_qu_ev_to_flt.empty()) {
      while (m_cv_valid_ev_to_flt.wait_for(lk_in, 10ms) == std::cv_status::timeout) {
        if (!m_is_running) {
          return n_df;
        }
      }
    }
    auto df = m_qu_ev_to_flt.front();
    m_qu_ev_to_flt.pop();
    lk_in.unlock();
    JadeDataFrameSP df_r = df;
    df_r->Decode();
    if (m_flt)
      df_r = m_flt->Filter(df);
    if (df_r) {
      std::unique_lock<std::mutex> lk_out_ana(m_mx_ev_to_ana);
      m_qu_ev_to_ana.push(df_r);
      lk_out_ana.unlock();
      m_cv_valid_ev_to_ana.notify_all();
      n_df++;
    }
  }
  return n_df;
}

uint64_t JadeManager::AsyncAnalysising()
{
  uint64_t n_df = 0;
  while (m_is_running) {
    std::unique_lock<std::mutex> lk_in(m_mx_ev_to_ana);
    while (m_qu_ev_to_ana.empty()) {
      while (m_cv_valid_ev_to_ana.wait_for(lk_in, 100ms)
          == std::cv_status::timeout) {
        if (!m_is_running) {
          return n_df;
        }
      }
    }
    if (m_qu_ev_to_ana.size() > 200000) {
      std::cerr << "JadeManager:: the data queue to Analysis is full\n";
      std::cerr << "JadeManager:: the Analysis is too slow to Analysis data\n";
      throw;
    }
    auto df = m_qu_ev_to_ana.front();
    m_qu_ev_to_ana.pop();
    lk_in.unlock();
    if (m_ana)
      m_ana->Analysis(df);
    n_df++;
  }
  return n_df;
}

void JadeManager::StartDataAnalysing()
{
  if (!m_rd || !m_flt || !m_ana) {
    std::cerr << "JadeManager: m_rd, m_flt or m_ana is not set" << std::endl;
    throw;
  }

  m_ana->Open();

  m_is_running = true;
  m_fut_async_rd = std::async(std::launch::async,
      &JadeManager::AsyncReading, this);

  m_fut_async_flt = std::async(std::launch::async,
      &JadeManager::AsyncFiltering, this);

  m_fut_async_ana = std::async(std::launch::async,
      &JadeManager::AsyncAnalysising, this);
}

void JadeManager::StopDataAnalysing()
{

  m_is_running = false;
  if (m_fut_async_rd.valid())
    m_fut_async_rd.get();
  std::cout << ">>>>>>>>>>>>>>>>>>end of rd" << std::endl;
  if (m_fut_async_flt.valid())
    m_fut_async_flt.get();
  std::cout << ">>>>>>>>>>>>>>>>>>end of flt" << std::endl;
  if (m_fut_async_ana.valid())
    m_fut_async_ana.get();
  std::cout << ">>>>>>>>>>>>>>>>>>end of ana" << std::endl;
  m_qu_ev_to_flt = decltype(m_qu_ev_to_flt)();
  m_qu_ev_to_ana = decltype(m_qu_ev_to_ana)();
  m_ana->Close();
}

void JadeManager::Reset()
{
  m_is_running = false;
  if (m_fut_async_rd.valid())
    m_fut_async_rd.get();
  if (m_fut_async_flt.valid())
    m_fut_async_flt.get();
  if (m_fut_async_ana.valid())
    m_fut_async_ana.get();
  m_qu_ev_to_flt = decltype(m_qu_ev_to_flt)();
  m_qu_ev_to_ana = decltype(m_qu_ev_to_ana)();
  if (m_rd)
    m_rd->Reset();
  if (m_flt)
    m_flt->Reset();
  if (m_ana)
    m_ana->Reset();
}

void JadeManager::DeviceConnect()
{
  m_rd->Open();
}

void JadeManager::DeviceDisconnect()
{
  m_rd->Close();
}
