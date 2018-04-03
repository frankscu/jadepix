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
    , m_is_seed_find(false)
{
}

JadeCluster::JadeCluster(JadeDataFrameSP df)
    : m_offset_y(0)
    , m_pixel_can_be_used({ true })
    , m_seed_thr(0)
    , m_cluster_thr(0)
    , m_neigh_thr(0)
    , m_size(0)
    , m_is_seed_find(false)
    , m_is_cluster_find(false)
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

bool JadeCluster::IsMax(size_t x, size_t y)
{
  auto adc_value = GetPixelADC(x, y);
  for (size_t ix = x - 1; ix <= x + 1; ix++)
    for (size_t iy = y - 1; iy <= y + 1; iy++)
      if (!IsInMatrix(ix, iy)) {
        continue;
      } else if (GetPixelADC(ix, iy) > adc_value) {
        return false;
      };

  return true;
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

void JadeCluster::FindSeed()
{
  // Reset pixel mask
  m_pixel_can_be_used.clear();
  m_pixel_can_be_used.resize(m_n_x * m_n_y, true);

  m_seed.clear();
  // Add pixel as seed candidate if it pass seed threshold
  for (size_t iy = 0; iy < m_n_y; iy++)
    for (size_t ix = 0; ix < m_n_x; ix++) {
      auto adc_value = GetPixelADC(ix, iy);
      // mask pixels that cannot pass neighbour threshold
      if (adc_value < m_neigh_thr) {
        SetPixelMask(ix, iy);
      } else if ((adc_value > m_seed_thr) && (IsMax(ix, iy))) {
        seed _seed;
        _seed.coord = std::make_pair(ix, iy);
        _seed.adc = adc_value;
        m_seed.push_back(_seed);
      }
    }

  // Sort seed pixels according to their ADC, hightest comes first
  std::sort(m_seed.begin(), m_seed.end(),
      [](auto& s1, auto& s2) { return s1.adc > s2.adc; });

  m_is_seed_find = true;
}

std::vector<std::pair<size_t, size_t> > JadeCluster::GetSeedCoord()
{
  if (!m_is_seed_find)
    FindSeed();

  std::vector<std::pair<size_t, size_t> > _seed_coord;

  _seed_coord.resize(m_seed.size());

  std::transform(m_seed.begin(), m_seed.end(), _seed_coord.begin(), [](auto& s) { return s.coord; });

  return _seed_coord;
}

int16_t JadeCluster::GetPixelADC(std::pair<size_t, size_t> coord)
{
  auto pos = (coord.first - m_offset_x) + m_n_x * (coord.second - m_offset_y);
  int16_t val = m_frame_adc.at(pos);
  return std::abs(val);
}

int16_t JadeCluster::GetPixelADC(size_t x, size_t y)
{
  auto pos = (x - m_offset_x) + m_n_x * (y - m_offset_y);
  int16_t val = m_frame_adc.at(pos);
  return std::abs(val);
}

std::vector<int16_t> JadeCluster::GetSeedADC()
{
  if (!m_is_seed_find)
    FindSeed();

  std::vector<int16_t> _seed_adc;

  _seed_adc.resize(m_seed.size());

  std::transform(m_seed.begin(), m_seed.end(), _seed_adc.begin(),
      [](auto& s) { return s.adc; });

  return _seed_adc;
}

void JadeCluster::FindCluster()
{
  auto seed_coord = GetSeedCoord();

  m_cluster.clear();

  for (auto& coord : seed_coord) {

    cluster _cluster;

    _cluster.total_adc = 0;

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

        _cluster.adc.push_back(GetPixelADC(ix, iy));
        _cluster.xCoord.push_back(ix);
        _cluster.yCoord.push_back(iy);
        _cluster.total_adc += GetPixelADC(ix, iy);

        // mask pixel that other cluster can use it
        SetPixelMask(ix, iy);
      }

    if (_cluster.total_adc > m_cluster_thr) {
      m_cluster.push_back(_cluster);
    }
  }

  m_is_cluster_find = true;
}

std::vector<JadeCluster::cluster> JadeCluster::GetCluster()
{
  if (!m_is_cluster_find)
    FindCluster();

  return m_cluster;
}

std::vector<int16_t> JadeCluster::GetClusterADC()
{
  auto cluster = GetCluster();

  std::vector<int16_t> _cluster_adc;

  _cluster_adc.resize(m_cluster.size());

  std::transform(cluster.begin(), cluster.end(), _cluster_adc.begin(),
      [](auto& c) { return c.total_adc; });

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

  auto cluster = GetCluster();

  for (auto& clus : cluster) {

    double yCenter = 0;
    double sum_y_adc = 0;

    for (auto& iy : clus.yCoord) {
      double y_adc = 0;
      for (auto& ix : clus.xCoord) {
        y_adc += GetPixelADC(ix, iy);
      }
      yCenter += iy * y_adc;
      sum_y_adc += y_adc;
    }

    yCenter = yCenter / sum_y_adc;
    _yCenter_Vec.push_back(yCenter);
  }

  return _yCenter_Vec;
}

std::vector<double> JadeCluster::GetXCenterOfGravity()
{
  std::vector<double> _xCenter_Vec;

  auto cluster = GetCluster();

  for (auto& clus : cluster) {

    double xCenter = 0;
    double sum_x_adc = 0;

    for (auto& ix : clus.xCoord) {
      double x_adc = 0;
      for (auto& iy : clus.yCoord) {
        x_adc += GetPixelADC(ix, iy);
      }
      xCenter += ix * x_adc;
      sum_x_adc += x_adc;
    }

    xCenter = xCenter / sum_x_adc;
    _xCenter_Vec.push_back(xCenter);
  }

  return _xCenter_Vec;
}
