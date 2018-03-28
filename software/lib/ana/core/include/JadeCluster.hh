#ifndef JADEPIX_JADECLUSTER
#define JADEPIX_JADECLUSTER

#include "JadeSystem.hh"

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class DLLEXPORT JadeCluster {
  public:
    JadeCluster();
    void set_values(int,std::vector<int>,std::vector<int>);
    JadeCluster mirrorX();
    JadeCluster mirrorY();
    JadeCluster rotate90();
    void NeighbourPixels(int x, int y, std::vector<int> xOriginal, std::vector<int> yOriginal, std::vector<int> &xNeighbour, std::vector<int> &yNeighbour);
    void FindReferenceClusters(std::vector<JadeCluster> &clusterVec, int sizeMax);
    std::map<int,int> SymmetryPairs(std::vector<JadeCluster> clusterVec, const char* type);
    std::vector< std::vector<int> > sameShape(std::vector<JadeCluster> clusterVec);
    int WhichClusterShape(JadeCluster cluster, std::vector<JadeCluster> clusterVec);
    std::vector<int> getX() {return m_x;}
    std::vector<int> getY() {return m_y;}
    int Size() {return m_size;}
    bool operator==(JadeCluster c2); 
    void getCenterOfGravity(float &xCenter, float &yCenter);
 protected:
    int m_size;
    std::vector<int> m_x;
    std::vector<int> m_y;
};

using JadeClusterSP = std::shared_ptr<JadeCluster>;

#endif
