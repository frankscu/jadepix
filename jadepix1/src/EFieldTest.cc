#include "EField.hh"
#include <iomanip>

void print_usage(){
  std::cout << "NAME\n\tEField\n" << std::endl;
  std::cout << "\nEField\n\t"
            << "\n-f\n\tInput electric field files" 
            << "\n-NX\n\tx dimensions\n"
            << "\n-NY\n\ty dimensions\n"
            << "\n-NZ\n\tz dimensions\n"
            << "\n-cutX\n\tx projection\n"
            << "\n-cutY\n\ty projection\n"
            << "\n-cutZ\n\tz projection\n"
            << "\n-o\n\tOutput root files\n"
            << std::endl; 
}

int main(int argc, char** argv) {
  if (argc < 1) {
    print_usage() ;
    return -1;
  }
  
  std::string opt_file = "field/jadepix1_electric_field.init";
  std::string opt_nx = "33";
  std::string opt_ny = "33";
  std::string opt_nz = "15";
  std::string opt_outfile = "output/jadepix1_electric_field_mag.root";
  std::string opt_cutX = "1";
  std::string opt_cutY = "1";
  std::string opt_cutZ = "1";

  for(int i=1;  i<argc; i++){
    std::string opt(argv[i]);
    if(opt=="-f"){
      if(i+1<argc){
        i++;
        opt_file = argv[i];
      }
    }
    if(opt=="-NX"){
      if(i+1<argc){
        i++;
        opt_nx = argv[i];
      }
    }
    if(opt=="-NY"){
      if(i+1<argc){
        i++;
        opt_ny = argv[i];
      }
    }
    if(opt=="-NZ"){
      if(i+1<argc){
        i++;
        opt_nz = argv[i];
      }
    }
    if(opt=="-o"){
      if(i+1<argc){
        i++;
        opt_outfile = argv[i];
      }
    }
    if(opt=="-cutX"){
      if(i+1<argc){
        i++;
        opt_cutX = argv[i];
      }
    }
    if(opt=="-cutY"){
      if(i+1<argc){
        i++;
        opt_cutY = argv[i];
      }
    }
    if(opt=="-cutZ"){
      if(i+1<argc){
        i++;
        opt_cutZ = argv[i];
      }
    }
  }



  std::string inputFile = std::string(opt_file);
  std::cout << "Input: " << inputFile << std::endl;

  size_t NX = std::stoul(opt_nx);
  size_t NY = std::stoul(opt_ny);
  size_t NZ = std::stoul(opt_nz);
  int cutX = std::stoul(opt_cutX);
  int cutY = std::stoul(opt_cutY);
  int cutZ = std::stoul(opt_cutZ);

  ROOT::Math::XYZVector cut(cutX,cutY,cutZ);

  auto pEField = new EField();

  auto fieldData = pEField->caculateEField(inputFile);

  pEField->setElectricField(fieldData);

  ROOT::Math::XYZPoint pos;
  std::vector<ROOT::Math::XYZPoint> posVec;

  for(size_t ix=0; ix<NX; ix++)
    for(size_t iy=0;iy<NY; iy++)
      for(size_t iz=0; iz<NZ; iz++){
        pos.SetXYZ(ix,iy,iz);
        posVec.push_back(pos);

        auto fieldVector = pEField->getElectricField(pos);
        
        std::cout << "Point ( " <<ix<<", "<<iy<< ", "<<iz <<") -> EField "
          << "( " <<fieldVector.X() << ", "
          <<fieldVector.Y() << ", "
          <<fieldVector.Z() << ") "
          << " Magenitude: " << TMath::Sqrt(fieldVector.Mag2())
          << std::endl;
      }

 pEField->drawEFieldXY(posVec,cut,opt_outfile);

  delete pEField;
}

