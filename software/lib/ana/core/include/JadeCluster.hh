#ifndef JADE_JADECLUSTER_HH
#define JADE_JADECLUSTER_HH

#include "JadeDataFrame.hh"
#include "JadeSystem.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

class DLLEXPORT JadeCluster {
  public:
  JadeCluster();
  JadeCluster(JadeDataFrameSP df);
  virtual ~JadeCluster(); 
  void SetSeedTHR(int16_t thr);
  void SetClusterTHR(int16_t thr);
  void SetNeighbourTHR(int16_t thr);
  void SetClusterSize(size_t size);
  bool IsInEdge(size_t x, size_t y) const;
  bool IsInMatrix(size_t x, size_t y) const;
  bool IsInMask(size_t x, size_t y) const;
  void SetPixelMask(size_t x, size_t y);
  int16_t GetPixelADC(size_t x, size_t y);
  virtual std::vector<std::pair<size_t, size_t> > GetSeedCoord();
  virtual std::vector<int16_t> GetSeedADC();
  virtual std::vector<int16_t> GetClusterADC();
  virtual std::vector<std::pair<double, double> > GetCenterOfGravity();
  virtual std::vector<double> GetXCenterOfGravity();
  virtual std::vector<double> GetYCenterOfGravity();

  private:
  uint16_t m_offset_x;
  uint16_t m_offset_y;
  uint16_t m_n_x;
  uint16_t m_n_y;
  std::vector<int16_t> m_frame_adc;
  std::vector<bool>  m_pixel_can_be_used;
  int16_t m_seed_thr;
  int16_t m_cluster_thr;
  int16_t m_neigh_thr;
  size_t m_size;
};

using JadeClusterSP = std::shared_ptr<JadeCluster>;

#endif
