#include "JadeFilter.hh"

using namespace std::chrono_literals;

JadeFilter::JadeFilter(const JadeOption& opt)
    : m_opt(opt)
    , m_ev_n(0)
    , m_last_df({0})
{
}

JadeFilter::~JadeFilter()
{
}

void JadeFilter::Reset()
{
}

JadeDataFrameSP JadeFilter::Filter(JadeDataFrameSP df)
{
  m_ev_n++;

  JadeDataFrameSP cds_df;

  if (m_ev_n % 2 == 1)
    m_last_df = df;
  if (m_ev_n % 2 == 0) {
    cds_df = std::make_shared<JadeDataFrame>(*df - *m_last_df);
  }

  return cds_df;
}
