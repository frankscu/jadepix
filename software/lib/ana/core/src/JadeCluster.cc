#include "JadeCluster.hh"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

JadeCluster aCluster;

JadeCluster::JadeCluster():m_size(0),
                           m_x(0),
                           m_y(0)
{
} 

bool JadeCluster::operator==(JadeCluster c2) {
  int x1Min = *min_element(m_x.begin(), m_x.end());                       
  int x2Min = *min_element(c2.m_x.begin(), c2.m_x.end());                 
  int y1Min = *min_element(m_y.begin(), m_y.end());                       
  int y2Min = *min_element(c2.m_y.begin(), c2.m_y.end());                 
  if (m_size != c2.Size()) return false;                                
  bool isEqual = false;                                               
  for (int i=0; i<m_size; i++)                                          
  {                                                                   
    for (int j=0; j<m_size; j++)                                        
    {                                                                 
      if (m_x[i]-x1Min == c2.m_x[j]-x2Min && m_y[i]-y1Min == c2.m_y[j]-y2Min) 
      {                                                               
        isEqual = true;                                               
        break;                                                        
      }                                                               
    }                                                                 
    if (isEqual)                                                      
    {                                                                 
      isEqual = false;                                                
      continue;                                                       
    }                                                                 
    else return false;                                                
  }                                                                   
  return true;                                                        
}                                                                     

JadeCluster JadeCluster::mirrorX() {
  std::vector<int> xNew(m_size);
  std::vector<int> yNew(m_size);
  int yMax = *max_element(m_y.begin(), m_y.end());
  for (int i=0; i<m_size; i++)
  {
    xNew[i]=m_x[i];
    yNew[i]=yMax-m_y[i];
  }
  JadeCluster clusterNew;
  clusterNew.set_values(m_size,xNew,yNew);
  return clusterNew;
}

JadeCluster JadeCluster::mirrorY() {
  std::vector<int> xNew(m_size);
  std::vector<int> yNew(m_size);
  int xMax = *max_element(m_x.begin(), m_x.end());
  for (int i=0; i<m_size; i++)
  {
    xNew[i]=xMax-m_x[i];
    yNew[i]=m_y[i];
  }
  JadeCluster clusterNew;
  clusterNew.set_values(m_size,xNew,yNew);
  return clusterNew;
}

JadeCluster JadeCluster::rotate90(){
  std::vector<int> xNew(m_size);
  std::vector<int> yNew(m_size);
  for (int i=0; i<m_size; i++)
  {
    xNew[i]=-1*m_y[i];
    yNew[i]=m_x[i];
  }
  int xMin = *min_element(xNew.begin(), xNew.end());
  int yMin = *min_element(yNew.begin(), yNew.end());
  for (int i=0; i<m_size; i++)
  {
    xNew[i]-=xMin;
    yNew[i]-=yMin;
  }
  JadeCluster clusterNew;
  clusterNew.set_values(m_size,xNew,yNew);
  return clusterNew;

}


void JadeCluster::set_values (int s, std::vector<int> a, std::vector<int> b) {
  m_size = s;
  if (m_x.size() == 0)
    for (int i=0; i<m_size; i++)
    {
      m_x.push_back(a[i]);
      m_y.push_back(b[i]);
    }
  else 
    for (int i=0; i<m_size; i++)
      if (m_x.size() <= (unsigned int)m_size)
      {
        m_x[i] = a[i];
        m_y[i] = b[i];
      }
      else
      {
        m_x.push_back(a[i]);
        m_y.push_back(b[i]);
      }
}

void JadeCluster::NeighbourPixels(int x, int y, std::vector<int> xOriginal, std::vector<int> yOriginal, std::vector<int> &xNeighbour, std::vector<int> &yNeighbour)
{
  int yTmp = 0;
  for (int xTmp=x-1; xTmp<=x+1; xTmp++)
  {
    bool pixelExists = false;
    if (xTmp==x-1 || xTmp==x+1) 
    {
      yTmp = y;
      for (unsigned int i=0; i<xOriginal.size(); i++)
      {
        if (xTmp == xOriginal[i] && yTmp == yOriginal[i]) 
        {
          pixelExists = true;
          break;
        }
      }
      if (pixelExists) continue;
      xNeighbour.push_back(xTmp);
      yNeighbour.push_back(yTmp);
      pixelExists = false;
    }
    else
    {
      for (int j=0; j<2; j++)
      {
        yTmp = (j==0?y+1:y-1);
        for (unsigned int i=0; i<xOriginal.size(); i++)
        {
          if (xTmp == xOriginal[i] && yTmp == yOriginal[i]) 
          {
            pixelExists = true;
            break;
          }
        }
        if (pixelExists) 
        {
          pixelExists = false; 
          continue;
        }
        xNeighbour.push_back(xTmp);
        yNeighbour.push_back(yTmp);
        pixelExists = false;
      }
    }
  }
}

void JadeCluster::FindReferenceClusters(std::vector<JadeCluster> &clusterVec, int sizeMax)
{
  std::vector<int> xTmp(1);
  std::vector<int> yTmp(1);
  xTmp[0]=0;
  yTmp[0]=0;
  JadeCluster cTmp;
  cTmp.set_values(1,xTmp,yTmp);
  clusterVec.push_back(cTmp);
  for (int size=2; size<=sizeMax; size++)
  {
    std::cout << "Looking for clusters with size: " << size << std::endl;
    int clusterVecSize = clusterVec.size();
    for (int iCluster=0; iCluster<clusterVecSize; iCluster++)
    {
      if (clusterVec[iCluster].Size() < size-1) continue;
      JadeCluster cluster; 
      std::vector<int> x = clusterVec[iCluster].getX();
      std::vector<int> y = clusterVec[iCluster].getY();
      for (int iPixel=0; iPixel<clusterVec[iCluster].Size(); iPixel++)
      {
        std::vector<int> xNeighbour;
        std::vector<int> yNeighbour;
        NeighbourPixels(x[iPixel],y[iPixel],x,y,xNeighbour,yNeighbour);
        for (unsigned int i=0; i<xNeighbour.size();i++)
        {
          bool areadyExists = false;
          if (x.size() < (unsigned int)size) 
          {
            x.push_back(xNeighbour[i]);
            y.push_back(yNeighbour[i]);
          }
          else
          {
            x[size-1] = xNeighbour[i];
            y[size-1] = yNeighbour[i];
          }
          cluster.set_values(size,x,y);
          for (unsigned int k=0; k<clusterVec.size(); k++)
            if (cluster == clusterVec[k]) 
            {
              areadyExists = true; 
              break;
            }
          if (!areadyExists)
            clusterVec.push_back(cluster);
        }
      }
    }
  }
  for (unsigned int iCluster = 0; iCluster<clusterVec.size();iCluster++)
  {
    JadeCluster cluster = clusterVec[iCluster];
    std::vector<int> X = cluster.getX();
    int minX = *min_element(X.begin(),X.end());
    if (minX < 0 )
      for (unsigned int iPixel=0; iPixel<X.size(); iPixel++)
        X[iPixel] -= minX;
    std::vector<int> Y = cluster.getY();
    int minY = *min_element(Y.begin(),Y.end());
    if (minY < 0 )
      for (unsigned int iPixel=0; iPixel<Y.size(); iPixel++)
        Y[iPixel] -= minY;
    cluster.set_values(X.size(),X,Y);
    clusterVec[iCluster] = cluster;
  }
  std::cout << "All shapes found!" << std::endl;
}

std::map<int,int> JadeCluster::SymmetryPairs(std::vector<JadeCluster> clusterVec, const char* type){
  std::map<int,int> pair;
  const char* typeX = "x";
  const char* typeY = "y";
  if (type != typeX && type != typeY)   
  {
    std::cerr << "Type has to be y or x, assuming x" << std::endl;
    type = "x";
  }
  for (unsigned int i=0; i<clusterVec.size(); i++)
  {
    JadeCluster cluster;
    if (type == typeX) cluster = clusterVec[i].mirrorX();
    else if (type == typeY) cluster = clusterVec[i].mirrorY();
    for (unsigned int j=0; j<clusterVec.size(); j++)
    {
      bool alreadyAdded = false;
      for(std::map<int,int>::iterator it = pair.begin(); it != pair.end(); ++it)
        if (it->first == (int)j)
        {
          alreadyAdded = true;
          break;
        }
      if (alreadyAdded) continue;
      if (cluster == clusterVec[j]) 
      {
        if (i == j) break;
        pair.insert(std::make_pair(i,j));
      }
    }
  }
  return pair;
}

std::vector< std::vector<int> > JadeCluster::sameShape(std::vector<JadeCluster> clusterVec){
  std::vector< std::vector<int> > symmetryGroups;
  for (unsigned int i=0; i<clusterVec.size(); i++)
  {
    std::vector<int> group;
    JadeCluster clusterOriginal = clusterVec[i];
    bool alreadyAdded = false;
    for (unsigned int k=0; k<symmetryGroups.size(); k++)
    {
      for (unsigned int l=0; l<symmetryGroups[k].size(); l++)
        if (symmetryGroups[k][l] == (int)i)
        {
          alreadyAdded = true;
          break;
        }
      if (alreadyAdded) break;
    }
    if (alreadyAdded) continue;
    else group.push_back(i);
    JadeCluster cluster2;
    for (int mir=0; mir<2; mir++)
    {
      if (mir == 0)  cluster2 = clusterOriginal;
      else  cluster2 = clusterOriginal.mirrorX();
      for (int rot=0; rot<4; rot++)
      {
        cluster2 = cluster2.rotate90();
        if (cluster2 == clusterOriginal) break;
        for (unsigned int j=0; j<clusterVec.size(); j++)
        {
          if (clusterVec[j] == cluster2)
          {
            alreadyAdded = false;
            for (unsigned int k=0; k<group.size(); k++)
              if (group[k] == (int)j)
              {
                alreadyAdded = true;
                break;
              }
            for (unsigned int k=0; k<symmetryGroups.size() && !alreadyAdded; k++)
            {
              for (unsigned int l=0; l<symmetryGroups[k].size(); l++)
                if (symmetryGroups[k][l] == (int)j)
                {
                  alreadyAdded = true;
                  break;
                }
              if (alreadyAdded) break;
            }
            if (!alreadyAdded) group.push_back(j);
          }
        }
      }
    }
    sort(group.begin(), group.end());
    symmetryGroups.push_back(group);
  }
  return symmetryGroups;
}


int JadeCluster::WhichClusterShape(JadeCluster cluster, std::vector<JadeCluster> clusterVec)
{
  for (unsigned int iCluster=0; iCluster<clusterVec.size(); iCluster++)
    if (cluster == clusterVec[iCluster]) return iCluster;
  return -1;
}

void JadeCluster::getCenterOfGravity(float &xCenter, float &yCenter)
{
  xCenter = 0;
  yCenter = 0;
  for (unsigned int iX=0; iX<m_x.size(); iX++)
    xCenter += m_x[iX];
  xCenter = xCenter/m_x.size();
  for (unsigned int iY=0; iY<m_y.size(); iY++)
    yCenter += m_y[iY];
  yCenter = yCenter/m_y.size();
}
