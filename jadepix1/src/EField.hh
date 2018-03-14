#ifndef EFIELD_HH
#define EFIELD_HH

#include <fstream>
#include <string>
#include <vector> 
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <Math/Point3D.h>
#include <Math/Rotation3D.h>
#include <Math/Translation3D.h>
#include <Math/Vector3D.h>
#include "TMath.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TStyle.h"

class EField {
  using FieldData = std::pair<std::shared_ptr<std::vector<double>>, std::array<size_t,3>>;
  public:
    EField();
    ~EField();

    FieldData caculateEField(std::string filename);
    void setElectricField(FieldData);
    ROOT::Math::XYZVector getElectricField(const ROOT::Math::XYZPoint& pos) const;
    void drawEFieldXY(std::vector<ROOT::Math::XYZPoint>& pos,ROOT::Math::XYZVector& cut, std::string& outfile);

  private:
    std::shared_ptr<std::vector<double>> m_electric_field;
    std::array<size_t,3> m_electric_field_sizes;
    int m_pixelsize_x;
    int m_pixelsize_y;
    int m_thickness;
    double m_depletion_depth; //defaut to full sensor
    double m_sensorCenterZ;
};

#endif //EFIELD_HH
