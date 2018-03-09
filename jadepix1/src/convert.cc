#include <fstream>
#include <string>
#include <vector> 
#include <iostream>
#include <memory>
#include <sstream>

void convert(std::string file_name, std::string outfile_name){
  std::ifstream file(file_name);

  if(file.fail()) {
    std::cerr << "invalid data or unexpected end of file" << std::endl;
    return;
  }

  std::ofstream outfile(outfile_name); 
  if(outfile.fail()) {
    std::cerr << "invalid file" << std::endl;
    return;
  }

  std::string header;
  std::getline(file, header);
  outfile << header << '\n';

  std::cout << "Header of file " << file_name << " is " << header << std::endl;

  std::string tmpline;
  std::getline(file, tmpline);
  int nlines=0;

  double thickness, xpixsz, ypixsz, temperature, flux, rhe, new_drde, tmp;
  size_t xsize, ysize, zsize;
  size_t xind, yind, zind;
  double xvalue, yvalue, zvalue;
  std::stringstream ss;
  while(std::getline(file, tmpline)){
    // print the init seed and cluster length
    // print the incident pion direction
    // print the magnetic field (specify separately)
    if(nlines<3){ 
      outfile << tmpline << '\n';
    }

    // Get information of electric field
    if(nlines == 3){
      ss.clear();
      ss.str(tmpline);
      ss >> xpixsz >> ypixsz >> thickness >> temperature >> flux >> rhe >> new_drde >> xsize >> ysize >> zsize >> tmp;
      outfile << thickness << " " << xpixsz << " " << ysize << " " << temperature << " " << flux << " " << rhe << " " << new_drde << " " << zsize << " " << xsize << " " << ysize << " " << tmp << '\n';
    }

    // Get index of electric field
    if(nlines > 3){
      ss.clear();
      ss.str(tmpline);
      ss >> yind >> zind >> xind >> yvalue >> zvalue >> xvalue;
      outfile << xind << " " << yind << " " << zind << " " << xvalue << " " << yvalue << " " << zvalue << '\n';
    }
    nlines++;
  }
  file.close();
  outfile.close();
}


void print_usage(){
  std::cout << "NAME\n\tconvert - convert geometry\n" << std::endl;
  std::cout << "\nSYNOPSIS\n\tconvert input output\n " << std::endl; 
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage() ;  
    return -1; 
  }

  std::string inputFile = std::string(argv[1]);
  std::string outputFile = std::string(argv[2]);

  std::cout << "Input: " << inputFile << std::endl; 
  convert(inputFile, outputFile);

  std::cout << "File saved as: " << outputFile << std::endl;

}

