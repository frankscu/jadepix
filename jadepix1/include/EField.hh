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
    FieldData caculateEField(std::string filename, bool isRot);
    void setElectricField(FieldData fieldData);
    ROOT::Math::XYZVector getElectricField(const ROOT::Math::XYZPoint& pos) const;
    void drawEFieldXY(std::vector<ROOT::Math::XYZPoint>& pos,ROOT::Math::XYZVector& cut, std::string& outfile);
    void setDepletionDepth(double depletion);
    void setPixelPitch(double x, double y, double z);
    void setPixelDiv(size_t x, size_t y, size_t z);
    void setPixelCenter(double x, double y, double z);
  private:
    std::shared_ptr<std::vector<double>> m_electric_field;
    std::array<size_t,3> m_electric_field_sizes;
    int m_pixelsize_x;
    int m_pixelsize_y;
    int m_thickness;
    double m_depletion_depth; //defaut to full sensor
    double m_sensorCenter_x;
    double m_sensorCenter_y;
    double m_sensorCenter_z;
    size_t m_pixeldiv_x;
    size_t m_pixeldiv_y;
    size_t m_pixeldiv_z;
};

#endif //EFIELD_HH
