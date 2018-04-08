#include "EFGen.hh"
using namespace mesh_converter;

void interrupt_handler(int) {
  //std::cout << "Interrupted! Aborting conversion...";
  std::exit(0);
}

void MeshElement::setVertices(std::vector<Point>& new_vertices) {
    if(vertices.size() != new_vertices.size()) {
      std::cout << "Invalid vertices vector";
      return;
    }
    for(size_t index = 0; index < new_vertices.size(); index++) {
        vertices[index] = new_vertices[index];
    }
}

void MeshElement::setVertex(size_t index, Point& new_vertice) {
    vertices[index] = new_vertice;
}

Point MeshElement::getVertex(size_t index) {
    return vertices[index];
}

void MeshElement::setVerticesField(std::vector<Point>& new_observable) {
    if(vertices.size() != new_observable.size()) {
      std::cout << "Invalid field vector";
        return;
    }
    for(size_t index = 0; index < 4; index++) {
        e_field[index] = new_observable[index];
    }
}

void MeshElement::setVertexField(size_t index, Point& new_observable) {
    e_field[index] = new_observable;
}

Point MeshElement::getVertexProperty(size_t index) {
    return e_field[index];
}

void MeshElement::setDimension(int dimension) {
    _dimension = dimension;
}

int MeshElement::getDimension() {
    return _dimension;
}

double MeshElement::getVolume() {
    double volume = 0;
    if(this->getDimension() == 3) {
        Eigen::Matrix4d element_matrix;
        element_matrix << 1, 1, 1, 1, vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x, vertices[0].y,
            vertices[1].y, vertices[2].y, vertices[3].y, vertices[0].z, vertices[1].z, vertices[2].z, vertices[3].z;
        volume = (element_matrix.determinant()) / 6;
    }
    if(this->getDimension() == 2) {
        Eigen::Matrix3d element_matrix;
        element_matrix << 1, 1, 1, vertices[0].y, vertices[1].y, vertices[2].y, vertices[0].z, vertices[1].z, vertices[2].z;
        volume = (element_matrix.determinant()) / 2;
    }
    return volume;
}

double MeshElement::getDistance(size_t index, Point& qp) {
    return unibn::L2Distance<Point>::compute(vertices[index], qp);
}

bool MeshElement::validElement(double volume_cut, Point& qp) {
    if(this->getVolume() == 0) {
      //std::cout << "Invalid tetrahedron with coplanar(3D)/colinear(2D) vertices." << std::endl;
        return false;
    }
    if(std::abs(this->getVolume()) <= volume_cut) {
      //std::cout << "Tetrahedron volume smaller than volume cut." << std::endl;
      return false;
    }

    Eigen::Matrix4d sub_tetra_matrix;
    for(size_t i = 0; i < static_cast<size_t>(this->getDimension()) + 1; i++) {
        std::vector<Point> sub_vertices = vertices;
        sub_vertices[i] = qp;
        MeshElement sub_tetrahedron(sub_vertices);
        sub_tetrahedron.setDimension(this->getDimension());
        double tetra_volume = sub_tetrahedron.getVolume();
        if(this->getVolume() * tetra_volume >= 0) {
            continue;
        }
        if(this->getVolume() * tetra_volume < 0) {
          //std::cout << "New mesh Point outside found element." << std::endl;
            return false;
        }
    }
    return true;
}

Point MeshElement::getObservable(Point& qp) {
    Point new_observable;
    Eigen::Matrix4d sub_tetra_matrix;
    for(size_t index = 0; index < static_cast<size_t>(this->getDimension()) + 1; index++) {
        auto sub_vertices = vertices;
        sub_vertices[index] = qp;
        MeshElement sub_tetrahedron(sub_vertices);
        sub_tetrahedron.setDimension(this->getDimension());
        double sub_volume = sub_tetrahedron.getVolume();
        //std::cout << "Sub volume " << index << ": " << sub_volume << std::endl;
        new_observable.x = new_observable.x + (sub_volume * e_field[index].x) / this->getVolume();
        new_observable.y = new_observable.y + (sub_volume * e_field[index].y) / this->getVolume();
        new_observable.z = new_observable.z + (sub_volume * e_field[index].z) / this->getVolume();
    }
    //std::cout << "Interpolated electric field: (" << new_observable.x << "," << new_observable.y << "," << new_observable.z
    //           << ")" << std::endl;
    return new_observable;
}

void MeshElement::printElement(Point& qp) {
    for(size_t index = 0; index < static_cast<size_t>(this->getDimension()) + 1; index++) {
      //std::cout    << "Tetrahedron vertex " << index_vec[index] << " (" << vertices[index].x << ", " << vertices[index].y
      //             << ", " << vertices[index].z << ") - "
      //             << " Distance: " << this->getDistance(index, qp) << " - Electric field: (" << e_field[index].x << ", "
      //             << e_field[index].y << ", " << e_field[index].z << ")"  
      //             << std::endl;
    }
    //std::cout << "Volume: " << this->getVolume() << std::endl;
}

void print_usage(){
  std::cout << "NAME\n\tEFRead\n" << std::endl;
  std::cout << "\nEFRead\n\t"
            << "\n-f\n\tInput files basename(*.dat *.grd)\n"
            << std::endl; 
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage() ;
    return -1;
  }
  
  std::string opt_file = "field/n2_dfise.init";
  std::string opt_region = "Epi";             
  std::string opt_observable = "ElectricField";
  std::string opt_init_file_prefix = "field/n2_dfise_converter";
  std::string opt_dimension = "3";
  std::string opt_nx = "33";
  std::string opt_ny = "33";
  std::string opt_nz = "15";
  std::string opt_radius = "1";
  std::string opt_max_radius = "10";
  std::string opt_radius_threshold = "0";
  bool threshold_flag = false;
  std::string opt_radius_step = "0.5";
  std::string opt_index_cut = "10000000";
  bool index_cut_flag = false;
  std::string opt_volume_cut = "1e-9";

  for(int i=1;  i<argc; i++){
    std::string opt(argv[i]);
    if(opt=="-f"){
      if(i+1<argc){
        i++;
        opt_file = argv[i];
      }
    }
    if(opt=="-R"){
      if(i+1<argc){
        i++;
        opt_region = argv[i];
      }
    }
    if(opt=="-O"){
      if(i+1<argc){
        i++;
        opt_observable = argv[i];
      }
    }
    if(opt=="-o"){
      if(i+1<argc){
        i++;
        opt_init_file_prefix = argv[i];
      }
    }
    if(opt=="-d"){
      if(i+1<argc){
        i++;
        opt_dimension = argv[i];
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
    if(opt=="-r"){
      if(i+1<argc){
        i++;
        opt_radius = argv[i];
      }
    }
    if(opt=="-m"){
      if(i+1<argc){
        i++;
        opt_max_radius = argv[i];
      }
    }
    if(opt=="-t"){
      if(i+1<argc){
        i++;
        opt_radius_threshold = argv[i];
        threshold_flag = true;
      }
    }
    if(opt=="-s"){
      if(i+1<argc){
        i++;
        opt_radius_step = argv[i];
      }
    }
    if(opt=="-i"){
      if(i+1<argc){
        i++;
        opt_index_cut = argv[i];
        index_cut_flag = true;
      }
    }
    if(opt=="-c"){
      if(i+1<argc){
        i++;
        opt_volume_cut = argv[i];
      }
    }
  }

  // Install abort handler (CTRL+\) and interrupt handler (CTRL+C)
  std::signal(SIGQUIT, interrupt_handler);
  std::signal(SIGINT, interrupt_handler);

  std::string file_prefix = std::string(opt_file);

  std::vector<std::string> region;
  std::vector<std::string>::iterator itr_region;

  EF::split_string(opt_region, region, "+"); 
  

  std::string observable = std::string(opt_observable);
  int dimension = static_cast<int>(std::stoul(opt_dimension));
  int xdiv = static_cast<int>(std::stoul(opt_nx));
  int ydiv = static_cast<int>(std::stoul(opt_ny));
  int zdiv = static_cast<int>(std::stoul(opt_nz));
  double initial_radius = static_cast<double>(std::stod(opt_radius));
  double max_radius = static_cast<double>(std::stod(opt_max_radius));
  double radius_threshold = static_cast<double>(std::stod(opt_radius_threshold));
  double radius_step = static_cast<double>(std::stod(opt_radius_step));
  size_t index_cut = static_cast<size_t>(std::stoul(opt_index_cut));
  double volume_cut = static_cast<double>(std::stod(opt_volume_cut));
  std::string init_file_prefix = std::string(opt_init_file_prefix); 

  std::cout << "Input: " << file_prefix << std::endl;
  for(itr_region=region.begin(); itr_region<region.end(); itr_region++){
    std::cout << "Region: " << (*itr_region) << std::endl;
  }  
  std::cout << "Observable: " << observable << std::endl;

  if(file_prefix.empty()) {
    print_usage();
    return 1;
  }

  std::string grid_file = file_prefix + ".grd";

  std::vector<Point> points;
  try {
    auto region_grid = read_grid(grid_file);
    for(itr_region=region.begin(); itr_region<region.end(); itr_region++){
      std::vector<Point> tmp_points = region_grid[(*itr_region)];
      points.insert(points.end(), tmp_points.begin(), tmp_points.end());
    }
  
  } catch(std::runtime_error& e) {
    std::cout << "Failed to parse grid file " << grid_file;
    std::cout << " " << e.what();
    return 1;
  }
  std::cout << "point size: " << points.size() << std::endl;

  std::string data_file = file_prefix + ".dat";
  std::vector<Point> field;
  try {
    auto region_fields = read_electric_field(data_file);
    for(itr_region=region.begin(); itr_region<region.end(); itr_region++){
      std::vector<Point> tmp_field = region_fields[(*itr_region)][observable];
      field.insert(field.end(), tmp_field.begin(), tmp_field.end());
    }
  } catch(std::runtime_error& e) {
    std::cout << "Failed to parse data file " << data_file;
    std::cout << " " << e.what();
    return 1;
  }
  std::cout << "field size: " << field.size() << std::endl;

  if(points.size() != field.size()) {
    std::cout << "Field and grid file do not match";
    return 1;
  }

  std::vector<std::pair<Point,Point>> pairPField;
  
  for(size_t i=0; i<points.size(); ++i){
    pairPField.push_back({points[i],field[i]});
  } 
  std::sort(pairPField.begin(),pairPField.end(),[](auto &l, auto &r){return l.first<r.first;});
  std::transform(pairPField.begin(),pairPField.end(),points.begin(),[](auto &m){return m.first;});
  std::transform(pairPField.begin(),pairPField.end(),field.begin(),[](auto &m){return m.second;});

  std::ofstream raw_file;
  std::stringstream raw_file_name;
  raw_file_name << init_file_prefix << "_" << observable << "_raw_data" << ".txt";
  raw_file.open(raw_file_name.str());

  // Write raw file header
  raw_file << "tcad_dfise_converter" << std::endl;                                  // NAME
  raw_file << "#Observable: " << observable << std::endl;                           // OBSERVABLE INTERPOLATED

  for(size_t i=0; i<points.size(); ++i){
    raw_file << pairPField[i].first.x << " " << pairPField[i].first.y << " " << pairPField[i].first.z << " " 
             << pairPField[i].second.x << " " << pairPField[i].second.y << " " << pairPField[i].second.z
             << std::endl;
  } 

  raw_file.close();


  //if(dimension == 3) {
  //  for(unsigned int i=0; i<points.size(); ++i){
  //    std::swap(points[i].y, points[i].z);
  //    std::swap(field[i].y, field[i].z);
  //  }
  //}

  double minx = DBL_MAX, miny = DBL_MAX, minz = DBL_MAX;
  double maxx = DBL_MIN, maxy = DBL_MIN, maxz = DBL_MIN;
  for(auto& point : points){
    maxx = std::max(maxx, point.x);
    maxy = std::max(maxy, point.y);
    maxz = std::max(maxz, point.z);
    minx = std::min(minx, point.x);
    miny = std::min(miny, point.y);
    minz = std::min(minz, point.z);
  }
  double xstep = (maxx - minx) / static_cast<double>(xdiv);
  double ystep = (maxy - miny) / static_cast<double>(ydiv);
  double zstep = (maxz - minz) / static_cast<double>(zdiv);
  double cell_volume = xstep * ystep * zstep;


  std::cout << "Mesh dimensions: " << maxx - minx << " x " << maxy - miny << " x " << maxz - minz << "\n"
            << "New mesh element dimension: " << xstep << " x " << ystep << " x " << zstep
            << " ==>  Volume = " << cell_volume << std::endl;

  //for(size_t i=0; i<points.size(); ++i){
  //  points[i].z = maxz - (points[i].z - minz);
  //  field[i].z = -field[i].z;
  //}

  // Initializing the Octree with points from mesh cloud.
  unibn::Octree<Point> octree;
  octree.initialize(points);
  std::vector<Point> e_field_new_mesh;

  double x = minx + xstep / 2.0;
  for(int i = 0; i < xdiv; ++i) {
    double y = miny + ystep / 2.0;
    for(int j = 0; j < ydiv; ++j) {
      double z = minz + zstep / 2.0;
      for(int k = 0; k < zdiv; ++k) {
        Point q, e;
        q.x = x;
        q.y = y;
        q.z = z; // New mesh vertex
        e.x = x;
        e.y = y;
        e.z = z; // Corresponding, to be interpolated, electric field

        bool valid = false;

        //std::cout << "Interpolating point X=" << i + 1 
        //          << " Y=" << j + 1 
        //          << " Z=" << k + 1 
        //          << " ("
        //          << q.x << "," 
        //          << q.y << "," 
        //          << q.z 
        //          << ")"
        //          << std::endl;

        size_t prev_neighbours = 0;
        double radius = initial_radius;
        size_t index_cut_up;
        while(radius < max_radius) {
          //std::cout << "Search radius: " << radius << std::endl;;
          // Calling octree neighbours search and sorting the results list with the closest neighbours first
          std::vector<unsigned int> results;
          std::vector<unsigned int> results_high;
          octree.radiusNeighbors<unibn::L2Distance<Point>>(q, radius, results_high);
          std::sort(results_high.begin(), results_high.end(), [&](unsigned int a, unsigned int b) {
              return unibn::L2Distance<Point>::compute(points[a], q) <
              unibn::L2Distance<Point>::compute(points[b], q);
              });

          if(threshold_flag) {
            size_t results_size = results_high.size();
            int count = 0;
            for(size_t index = 0; index < results_size; index++) {
              if(unibn::L2Distance<Point>::compute(points[results_high[index]], q) < radius_threshold) {
                count++;
                continue;
              }
              results.push_back(results_high[index]);
            }
            //std::cout << "Applying radius threshold of " << radius_threshold << "\n"
            //          << "Removing " << count << " of " << results_size << std::endl;
          } else {
            results = results_high;
          }

          // If after a radius step no new neighbours are found, go to the next radius step
          if(results.size() <= prev_neighbours || results.empty()) {
            prev_neighbours = results.size();
            //std::cout << "No (new) neighbour found with radius " << radius << ". Increasing search radius."
            //  << std::endl;
            radius = radius + radius_step;
            continue;
          }

          if(results.size() < 4) {
            //std::cout << "Incomplete mesh element found for radius " << radius << "\n"
            //  << "Increasing the readius (setting a higher initial radius may help)" << std::endl;
            radius = radius + radius_step;
            continue;
          }

          std::cout << "Number of vertices found: " << results.size() << std::endl;

          // Finding tetrahedrons
          Eigen::Matrix4d matrix;
          size_t num_nodes_element = 4;
          std::vector<Point> element_vertices;
          std::vector<Point> element_vertices_field;

          std::vector<int> bitmask(num_nodes_element, 1);
          bitmask.resize(results.size(), 0);
          std::vector<size_t> index;

          if(!index_cut_flag) {
            index_cut = results.size();
          }
          index_cut_up = index_cut;
          while(index_cut_up <= results.size()) {
            do {
              valid = false;
              index.clear();
              element_vertices.clear();
              element_vertices_field.clear();
              // print integers and permute bitmask
              for(size_t idk = 0; idk < results.size(); ++idk) {
                if(bitmask[idk] != 0) {
                  index.push_back(idk);
                  element_vertices.push_back(points[results[idk]]);
                  element_vertices_field.push_back(field[results[idk]]);
                                }
                                if(index.size() == num_nodes_element) {
                                    break;
                                }
                            }

                            bool index_flag = false;
                            for(size_t ttt = 0; ttt < num_nodes_element; ttt++) {
                                if(index[ttt] > index_cut_up) {
                                    index_flag = true;
                                    break;
                                }
                            }
                            if(index_flag) {
                                continue;
                            }

                            if(dimension == 3) {
                              //std::cout << "Parsing neighbors [index]: " << index[0] << ", " << index[1] << ", "
                              //             << index[2] << ", " << index[3] << std::endl;
                            }
                            if(dimension == 2) {
                              //std::cout
                              //      << "Parsing neighbors [index]: " << index[0] << ", " << index[1] << ", " << index[2] << std::endl;
                            }

                            MeshElement element(dimension, index, element_vertices, element_vertices_field);
                            valid = element.validElement(volume_cut, q);
                            if(!valid) {
                                continue;
                            }
                            element.printElement(q);
                            e = element.getObservable(q);
                            break;
                        } while(std::prev_permutation(bitmask.begin(), bitmask.end()));

                        if(valid) {
                            break;
                        }

                        //std::cout << "All combinations tried up to index " << index_cut_up
                        //           << " done. Increasing the index cut." << std::endl;
                        index_cut_up = index_cut_up + index_cut;
                    }

                    if(valid) {
                        break;
                    }

                    //std::cout << "All combinations tried. Increasing the radius." << std::endl;
                    radius = radius + radius_step;
                }

                if(!valid) {
                  //std::cout << "Couldn't interpolate new mesh point, probably the grid is too irregular" << std::endl;
                    return 1;
                }

                e_field_new_mesh.push_back(e);
                z += zstep;
            }
            y += ystep;
        }
        x += xstep;
    }

  std::cout << "Writing INIT file" << std::endl;

  std::ofstream init_file;
  std::stringstream init_file_name;
  init_file_name << init_file_prefix << "_" << observable << ".init";
  init_file.open(init_file_name.str());

  // Write INIT file h"eader
  init_file << "tcad_dfise_converter" << std::endl;                                  // NAME
  //init_file << "#Observable: " << observable << std::endl;                           // OBSERVABLE INTERPOLATED
  init_file << "##SEED## ##EVENTS##" << std::endl;                                   // UNUSED
  init_file << "##TURN## ##TILT## 1.0" << std::endl;                                 // UNUSED
  init_file << "0.0 0.0 0.0" << std::endl;                                           // MAGNETIC FIELD (UNUSED)
  init_file << (maxz - minz) << " " << (maxx - minx) << " " << (maxy - miny) << " "; // PIXEL DIMENSIONS
  init_file << "0.0 0.0 0.0 0.0 ";                                                   // UNUSED
  init_file << xdiv << " " << ydiv << " " << zdiv << " ";                            // GRID SIZE
  init_file << "0.0" << std::endl;                                                   // UNUSED

  // Write INIT file data
  for(int i = 0; i < xdiv; ++i) {
    for(int j = 0; j < ydiv; ++j) {
      for(int k = 0; k < zdiv; ++k) {
        auto& point = e_field_new_mesh[static_cast<unsigned int>(i * ydiv * zdiv + j * zdiv + k)];
        init_file << i + 1 << " " << j + 1 << " " << k + 1 << " " << point.x << " " << point.y << " " << point.z
          << std::endl;
      }
    }
  }
  init_file.close();

  return 1;
}
