#include "EField.hh"

EField::EField():m_pixelsize_x(33),
                 m_pixelsize_y(33),
                 m_thickness(15),
                 m_pixeldiv_x(33),
                 m_pixeldiv_y(33),
                 m_pixeldiv_z(15)
{
  m_depletion_depth = m_thickness;
  m_sensorCenter_z = m_thickness/2.0;
}

EField::~EField(){
}

EField::FieldData EField::caculateEField(std::string file_name){
  caculateEField(file_name, true);
}

EField::FieldData EField::caculateEField(std::string file_name, bool isRot){
  std::ifstream file(file_name);

  if(file.fail()) {
    std::cerr << "invalid data or unexpected end of file" << std::endl;
    throw std::runtime_error("invalid file");
  }

  std::string header;
  std::getline(file, header);

  std::cout << "Header of file " << file_name << " is " << header << std::endl;

  std::string tmp;
  if(!isRot) file >> tmp >> tmp;
  file >> tmp >> tmp;
  file >> tmp >> tmp >> tmp;
  file >> tmp >> tmp >> tmp;

  double thickness, xpixsz, ypixsz;
  file >> thickness >> xpixsz >> ypixsz;
  file >> tmp >> tmp >> tmp >> tmp;

  setPixelPitch(xpixsz, ypixsz, thickness);
  setPixelCenter(xpixsz/2, ypixsz/2, thickness/2);

  size_t xsize, ysize, zsize;
  file >> xsize >> ysize >> zsize;
  file >> tmp;

  std::cout << "Input XYZ size: " 
            << "( " << xsize << ", " << ysize << ", " << zsize << " )" 
            << std::endl;
  setPixelDiv(xsize, ysize, zsize);

  auto field = std::make_shared<std::vector<double>>(); 
  field->resize(xsize * ysize * zsize *3);

  for(size_t i=0; i< xsize  * ysize * zsize; ++i){
    if(file.eof()){
      throw std::runtime_error("unexpected end of file");
    }

    size_t xind, yind, zind;
    file >> xind >> yind >> zind; 
  
    if(file.fail() || xind> xsize || yind > ysize || zind>zsize){
      throw std::runtime_error("invalid data");
    } 
    xind--;
    yind--;
    zind--;

    for(size_t j=0; j<3; ++j){
      double input;
      file >> input;
      (*field)[xind * ysize * zsize * 3 + yind * zsize * 3 + zind * 3 +j] = input;
    }
  }
  file.close();

  return std::make_pair(field, std::array<size_t, 3>{{xsize, ysize, zsize}});
}

void EField::setElectricField(FieldData fieldData){
  m_electric_field = fieldData.first;
  m_electric_field_sizes = fieldData.second; 

  std::cout << "Set electric field with " 
            << fieldData.second.at(0) << " x " 
            << fieldData.second.at(1) << " x "
            << fieldData.second.at(2) << " cells"
            << std::endl;
}

ROOT::Math::XYZVector EField::getElectricField(const ROOT::Math::XYZPoint& pos) const{
  auto x = pos.x();
  auto y = pos.y();
  auto z = pos.z();
  
  std::cout << "Input XYZPoint: " 
            << "( " << x << ", " << y << ", " << z << " )" 
            << std::endl;

  auto sensor_max_z = m_sensorCenter_z + m_thickness/2.0;
  auto electric_field_thickness_domain = std::make_pair(sensor_max_z - m_depletion_depth, sensor_max_z); 

  std::cout << "Local XYPoint: " 
            << "( " << x << ", " << y << ")" 
            << "\nSensor Max Z: "
            << sensor_max_z
            << "\nElectric filed thickness domain: "
            << "( " << electric_field_thickness_domain.first << ", " << electric_field_thickness_domain.second << ")" 
            << std::endl;

  std::cout << "Electric field sizes: (" 
            << m_electric_field_sizes[0] << ", " 
            << m_electric_field_sizes[1] << ", "
            << m_electric_field_sizes[2] << ")" 
            << std::endl;

  ROOT::Math::XYZVector ret_val;

  auto x_ind = static_cast<int>(std::floor(static_cast<double>(m_electric_field_sizes[0]) *
              x / m_pixelsize_x));
  
  auto y_ind = static_cast<int>(std::floor(static_cast<double>(m_electric_field_sizes[1]) *
              y / m_pixelsize_y));
  
  auto z_ind = static_cast<int>(std::floor(static_cast<double>(m_electric_field_sizes[2]) * (z - electric_field_thickness_domain.first) /
      (electric_field_thickness_domain.second - electric_field_thickness_domain.first)));

  std::cout << " x_ind: " << x_ind << "; "
            << " y_ind: " << y_ind << "; "
            << " z_ind: " << z_ind             
            << std::endl;

  // Check for indices within the sensor
  if( x_ind < 0 || x_ind >= static_cast<int>(m_electric_field_sizes[0]) || 
      y_ind < 0 || y_ind >= static_cast<int>(m_electric_field_sizes[1]) || 
      z_ind < 0 || z_ind >= static_cast<int>(m_electric_field_sizes[2]) ) 
  {
    return ROOT::Math::XYZVector(0, 0, 0);
  }

  size_t tot_ind = static_cast<size_t>(x_ind) * m_electric_field_sizes[1] * m_electric_field_sizes[2] * 3 +
    static_cast<size_t>(y_ind) * m_electric_field_sizes[2] * 3 + static_cast<size_t>(z_ind) * 3;

  std::cout << " tot_ind: " << tot_ind << std::endl;

  ret_val = ROOT::Math::XYZVector(
      (*m_electric_field)[tot_ind], (*m_electric_field)[tot_ind + 1], (*m_electric_field)[tot_ind + 2]);

  return ret_val;
}

void EField::drawEFieldXY(std::vector<ROOT::Math::XYZPoint>& posVec,ROOT::Math::XYZVector& cut, std::string& outfile){
  
  auto tf = new TFile(outfile.c_str(), "RECREATE");
  auto exyfield_map = new TH2D("xy_efMap", 
                               "xy_efMap", 
                               m_pixeldiv_x, 
                               0, 
                               m_pixeldiv_x,
                               m_pixeldiv_y,
                               0,
                               m_pixeldiv_y);

  auto exzfield_map = new TH2D("xz_efMap", 
                               "xz_efMap", 
                               m_pixeldiv_x, 
                               0, 
                               m_pixeldiv_x,
                               m_pixeldiv_z,
                               0,
                               m_pixeldiv_z);

  auto eyzfield_map = new TH2D("yz_efMap", 
                               "yz_efMap", 
                               m_pixeldiv_y, 
                               0, 
                               m_pixeldiv_y,
                               m_pixeldiv_z,
                               0,
                               m_pixeldiv_z);

  auto exyzfield_map = new TH3D("xyz_efMap", 
                               "xyz_efMap", 
                               m_pixeldiv_x, 
                               0, 
                               m_pixeldiv_x,
                               m_pixeldiv_y, 
                               0, 
                               m_pixeldiv_y,
                               m_pixeldiv_z,
                               0,
                               m_pixeldiv_z);

  std::vector<ROOT::Math::XYZPoint>::iterator itpos;
  ROOT::Math::XYZVector efVec;

  for(itpos=posVec.begin(); itpos!=posVec.end(); itpos++){
    efVec = getElectricField(*itpos);
    if(itpos->Z()==cut.Z()){
      exyfield_map->Fill(itpos->X(), itpos->Y(), TMath::Sqrt(efVec.Mag2()));
    }
  
    if(itpos->Y()==cut.Y()){
      exzfield_map->Fill(itpos->X(), itpos->Z(), TMath::Sqrt(efVec.Mag2()));
    }
  
    if(itpos->X()==cut.X()){
      eyzfield_map->Fill(itpos->Y(), itpos->Z(), TMath::Sqrt(efVec.Mag2()));
    }
  
      exyzfield_map->Fill(itpos->X(), itpos->Y(), itpos->Z(), TMath::Sqrt(efVec.Mag2()));
  } 
 
  //TH3D BOX2 
  //TH2D COLZ/ARR 
  tf->Write(); 
}

void EField::setDepletionDepth(double depletion){
  m_depletion_depth = depletion;
}

void EField::setPixelPitch(double x, double y, double z){
  m_pixelsize_x = x;
  m_pixelsize_y = y;
  m_thickness = z;
}

void EField::setPixelDiv(size_t x, size_t y, size_t z){
  m_pixeldiv_x = x;
  m_pixeldiv_y = y;
  m_pixeldiv_z = z;
}

void EField::setPixelCenter(double x, double y, double z){
  m_sensorCenter_x = x;
  m_sensorCenter_y = y;
  m_sensorCenter_z = z;
}
