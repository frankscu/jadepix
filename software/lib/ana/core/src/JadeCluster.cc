#include "JadeCluster.hh"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

JadeCluster::JadeCluster()
    : m_offset_x(0)
    , m_offset_y(0)
    , m_n_x(0)
    , m_n_y(0)
    , m_frame_adc({ 0 })
    , m_pixel_can_be_used({ true })
    , m_seed_thr(0)
    , m_cluster_thr(0)
    , m_neigh_thr(0)
    , m_size(0)
{
}

JadeCluster::JadeCluster(JadeDataFrameSP df)
    : m_offset_x(0)
    , m_offset_y(0)
    , m_n_x(0)
    , m_n_y(0)
    , m_frame_adc({ 0 })
    , m_pixel_can_be_used({ true })
    , m_seed_thr(0)
    , m_cluster_thr(0)
    , m_neigh_thr(0)
    , m_size(0)
{
  m_offset_x = df->GetMatrixLowX();
  m_offset_y = df->GetMatrixLowY();
  m_n_x = df->GetMatrixSizeX();
  m_n_y = df->GetMatrixSizeY();
  m_frame_adc = df->GetFrameCDS();
}

JadeCluster::~JadeCluster()
{
}

void JadeCluster::SetSeedTHR(int16_t thr)
{
  m_seed_thr = thr;
}

void JadeCluster::SetClusterTHR(int16_t thr)
{
  m_cluster_thr = thr;
}

void JadeCluster::SetNeighbourTHR(int16_t thr)
{
  m_neigh_thr = thr;
}

void JadeCluster::SetClusterSize(size_t size)
{
  m_size = size;
}

bool JadeCluster::IsInEdge(size_t x, size_t y) const
{
  if (x - m_offset_x == m_n_x && y - m_offset_y == m_n_y && x == m_offset_x && y == m_offset_y)
    return true;
  else
    return false;
}

bool JadeCluster::IsInMatrix(size_t x, size_t y) const
{
  if (x - m_offset_x < m_n_x && y - m_offset_y < m_n_y && x >= m_offset_x && y >= m_offset_y)
    return true;
  else
    return false;
}

bool JadeCluster::IsInMask(size_t x, size_t y) const
{
  size_t pos = (x - m_offset_x) + m_n_x * (y - m_offset_y);
  if (m_pixel_can_be_used.at(pos))
    return false;
  else
    return true;
}

void JadeCluster::SetPixelMask(size_t x, size_t y)
{
  size_t pos = (x - m_offset_x) + m_n_x * (y - m_offset_y);
  m_pixel_can_be_used.at(pos) = false;
}

std::vector<std::pair<size_t, size_t> > JadeCluster::GetSeedCoord()
{
  // Reset pixel mask
  m_pixel_can_be_used.clear();
  m_pixel_can_be_used.resize(m_n_x * m_n_y, true);

  // Add pixel as seed candidate if it pass seed threshold
  std::vector<std::pair<size_t, size_t> > _seed_candidate;
  for (size_t iy = 0; iy < m_n_y; iy++)
    for (size_t ix = 0; ix < m_n_x; ix++) {
      auto adc_value = GetPixelADC(ix, iy);
      // mask pixels that cannot pass neighbour threshold
      if (adc_value < m_neigh_thr) {
        SetPixelMask(ix, iy);
      } else if (adc_value > m_seed_thr) {
        _seed_candidate.push_back(std::make_pair(ix, iy));
      }
    }

  // Sort seed pixels according to their ADC, hightest comes first
  for (size_t i = 0; i + 1 < _seed_candidate.size(); i++) {
    int16_t adc_value = GetPixelADC(_seed_candidate[i].first, _seed_candidate[i].second);
    int16_t next_adc_value = GetPixelADC(_seed_candidate[i + 1].first, _seed_candidate[i + 1].second);
    if (adc_value < next_adc_value) {
      _seed_candidate[i].swap(_seed_candidate[i + 1]);
      i = 0; // start from beginning
    }
  }

  return _seed_candidate;
}

int16_t JadeCluster::GetPixelADC(size_t x, size_t y)
{
  auto pos = (x - m_offset_x) + m_n_x * (y - m_offset_y);
  int16_t val = m_frame_adc.at(pos);
  return val;
}

std::vector<int16_t> JadeCluster::GetSeedADC()
{
  std::vector<int16_t> _seed_adc;
  auto seed_coord = GetSeedCoord();
  for (auto& coord : seed_coord) {
    _seed_adc.push_back(GetPixelADC(coord.first, coord.second));
  }
  return _seed_adc;
}

std::vector<int16_t> JadeCluster::GetClusterADC()
{
  std::vector<int16_t> _cluster_adc;

  auto seed_coord = GetSeedCoord();

  for (auto& coord : seed_coord) {

    int16_t adc_value = 0;

    for (size_t iy = static_cast<size_t>(coord.second - m_size / 2.0);
         iy <= static_cast<size_t>(coord.second + m_size / 2.0); iy++)
      for (size_t ix = static_cast<size_t>(coord.first - m_size / 2.0);
           ix <= static_cast<size_t>(coord.first + m_size / 2.0); ix++) {

        // skip pixel outside and in the edge
        if (!IsInMatrix(ix, iy) || IsInEdge(ix, iy))
          continue;

        // skip pixel that other cluster use it
        if (IsInMask(ix, iy))
          continue;

        adc_value += GetPixelADC(ix, iy);
        // mask pixel that other cluster can use it
        SetPixelMask(ix, iy);
      }

    if (adc_value > m_cluster_thr)
      _cluster_adc.push_back(adc_value);
  }
  return _cluster_adc;
}

std::vector<std::pair<double, double> > JadeCluster::GetCenterOfGravity()
{
  std::vector<std::pair<double, double> > _center;
  auto xCenter = GetXCenterOfGravity();
  auto yCenter = GetYCenterOfGravity();

  if (xCenter.size() == yCenter.size()) {
    _center.resize(xCenter.size());
    std::transform(xCenter.begin(), xCenter.end(),
        yCenter.begin(),
        _center.begin(),
        [](auto& x, auto& y) { return std::make_pair(x, y); });
  } else {
    std::cerr << " xCenter size is not equal to yCenter! " << std::endl;
    throw std::runtime_error("Invalid size");
  }

  return _center;
}

std::vector<double> JadeCluster::GetYCenterOfGravity()
{
  std::vector<double> _yCenter_Vec;
  auto seed_coord = GetSeedCoord();

  for (auto& coord : seed_coord) {

    double yCenter = 0;
    double sum_y_adc = 0;

    for (size_t iy = static_cast<size_t>(coord.second - m_size / 2.0);
         iy <= static_cast<size_t>(coord.second + m_size / 2.0); iy++) {
      double y_adc = 0;
      for (size_t ix = static_cast<size_t>(coord.first - m_size / 2.0);
           ix <= static_cast<size_t>(coord.first + m_size / 2.0); ix++) {

        if (!IsInMatrix(ix, iy) || IsInEdge(ix, iy))
          continue;

        if (IsInMask(ix, iy))
          continue;

        y_adc += GetPixelADC(ix, iy);
        SetPixelMask(ix, iy);
      }
      yCenter += iy * y_adc;
      sum_y_adc += y_adc;
    }

    if (sum_y_adc > m_cluster_thr) {
      yCenter = yCenter / sum_y_adc;
      _yCenter_Vec.push_back(yCenter);
    }
  }

  return _yCenter_Vec;
}

std::vector<double> JadeCluster::GetXCenterOfGravity()
{
  std::vector<double> _xCenter_Vec;

  auto seed_coord = GetSeedCoord();

  for (auto& coord : seed_coord) {

    double xCenter = 0;
    double sum_x_adc = 0;

    for (size_t ix = static_cast<size_t>(coord.first - m_size / 2.0);
         ix <= static_cast<size_t>(coord.first + m_size / 2.0); ix++) {
      double x_adc = 0;
      for (size_t iy = static_cast<size_t>(coord.second - m_size / 2.0);
           iy <= static_cast<size_t>(coord.second + m_size / 2.0); iy++) {

        if (!IsInMatrix(ix, iy) || IsInEdge(ix, iy))
          continue;

        if (IsInMask(ix, iy))
          continue;

        x_adc += GetPixelADC(ix, iy);
        SetPixelMask(ix, iy);
      }
      xCenter += ix * x_adc;
      sum_x_adc += x_adc;
    }

    if (sum_x_adc > m_cluster_thr) {
      xCenter = xCenter / sum_x_adc;
      _xCenter_Vec.push_back(xCenter);
    }
  }

  return _xCenter_Vec;
}
