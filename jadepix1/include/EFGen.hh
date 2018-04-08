#include <Eigen/Eigen>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <climits>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <Eigen/Eigen>

#include "EFReadDFISE.hh"
#include "EFString.hh"
#include "Octree.hpp"

// Interrupt handler
void interrupt_handler(int);

void print_usage();

namespace mesh_converter {

  class MeshElement {
    public:
      explicit MeshElement() = default;

      explicit MeshElement(std::vector<Point> vertices_tetrahedron) : vertices(std::move(vertices_tetrahedron)) {}

      MeshElement(Point& v1, Point& v2, Point& v3, Point& v4) : MeshElement(std::vector<Point>({v1, v2, v3, v4})) {}

      MeshElement(Point& v1, Point& f1, Point& v2, Point& f2, Point& v3, Point& f3, Point& v4, Point& f4)
        : MeshElement(std::vector<Point>({v1, v2, v3, v4}), std::vector<Point>({f1, f2, f3, f4})) {}

      MeshElement(std::vector<Point> vertices_tetrahedron, std::vector<Point> efield_vertices_tetrahedron)
        : vertices(std::move(vertices_tetrahedron)), e_field(std::move(efield_vertices_tetrahedron)) {}

      MeshElement(int dimension,
          std::vector<size_t> index,
          std::vector<Point> vertices_tetrahedron,
          std::vector<Point> efield_vertices_tetrahedron)
        : _dimension(dimension), index_vec(std::move(index)), vertices(std::move(vertices_tetrahedron)),
        e_field(std::move(efield_vertices_tetrahedron)) {}

      void setVertices(std::vector<Point>& new_vertices);

      void setVertex(size_t index, Point& new_vertice);

      Point getVertex(size_t index);

      void setVerticesField(std::vector<Point>& new_e_field);

      void setVertexField(size_t index, Point& new_e_field);

      Point getVertexProperty(size_t index);

      int getDimension();

      void setDimension(int dimension);

      double getVolume();

      double getDistance(size_t index, Point& qp);

      bool validElement(double volume_cut, Point& qp);

      Point getObservable(Point& qp);

      void printElement(Point& qp);


    private:
      int _dimension{3};
      std::vector<size_t> index_vec{};
      std::vector<Point> vertices{};
      std::vector<Point> e_field{};
  };
} // namespace mesh_converter
