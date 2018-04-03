#ifndef JADE_JADECLUSTER_HH
#define JADE_JADECLUSTER_HH

#include "JadeDataFrame.hh"
#include "JadeSystem.hh"

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cmath>

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
  bool IsMax(size_t x, size_t y);
  void SetPixelMask(size_t x, size_t y);
  int16_t GetPixelADC(size_t x, size_t y);
  int16_t GetPixelADC(std::pair<size_t, size_t> coord);
  std::vector<int16_t> GetSeedADC();
  std::vector<int16_t> GetClusterADC();
  virtual void FindSeed();
  virtual void FindCluster();
  virtual std::vector<std::pair<size_t, size_t> > GetSeedCoord();
  virtual std::vector<std::pair<double, double> > GetCenterOfGravity();
  virtual std::vector<double> GetXCenterOfGravity();
  virtual std::vector<double> GetYCenterOfGravity();

  private:
  struct seed {
    std::pair<size_t, size_t> coord;
    int16_t adc;
  };
  struct cluster {
    std::vector<size_t> xCoord;
    std::vector<size_t> yCoord;
    std::vector<int16_t> adc;
    int16_t total_adc;
  };
  std::vector<seed> GetSeed();
  std::vector<cluster> GetCluster();

  private:
  uint16_t m_offset_x;
  uint16_t m_offset_y;
  uint16_t m_n_x;
  uint16_t m_n_y;
  std::vector<int16_t> m_frame_adc;
  std::vector<bool> m_pixel_can_be_used;
  int16_t m_seed_thr;
  int16_t m_cluster_thr;
  int16_t m_neigh_thr;
  size_t m_size;
  std::vector<seed> m_seed;
  std::vector<cluster> m_cluster;
  bool m_is_seed_find;
  bool m_is_cluster_find;
};

using JadeClusterSP = std::shared_ptr<JadeCluster>;

#endif
