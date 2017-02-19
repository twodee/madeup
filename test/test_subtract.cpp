#include <iostream>
#define IGL_NO_CORK
#include <igl/copyleft/cgal/mesh_boolean.h>

int main(int argc, char **argv) {
  Eigen::MatrixXd big_positions(10, 3);
  big_positions <<
    -6.010407, 0, 6.010407,
    -6.010407, 2, 6.010407,
    -6.010407, 0, -6.010406,
    -6.010407, 2, -6.010406,
    0, 0, 0,
    0, 2, 0,
    6.010406, 0, -6.010407,
    6.010407, 0, 6.010407,
    6.010406, 2, -6.010407,
    6.010407, 2, 6.010407;

  Eigen::MatrixXd small_positions(10, 3);
  small_positions <<
    -4.949748, 1, -4.949747,
    -4.949747, 1, 4.949747,
    -4.949748, 3.5, -4.949747,
    -4.949747, 3.5, 4.949747,
    0, 1, 0,
    0, 3.5, 0,
    4.949747, 1, -4.949747,
    4.949747, 1, 4.949747,
    4.949747, 3.5, -4.949747,
    4.949747, 3.5, 4.949747;

  Eigen::MatrixXi big_faces(16, 3);
  big_faces <<
    7, 6, 8,
    7, 8, 9,
    6, 2, 3,
    6, 3, 8,
    2, 0, 1,
    2, 1, 3,
    0, 7, 9,
    0, 9, 1,
    4, 6, 7,
    4, 2, 6,
    4, 0, 2,
    4, 7, 0,
    5, 9, 8,
    5, 8, 3,
    5, 3, 1,
    5, 1, 9;

  Eigen::MatrixXi small_faces(16, 3);
  small_faces <<
    7, 6, 8,
    7, 8, 9,
    6, 0, 2,
    6, 2, 8,
    0, 1, 3,
    0, 3, 2,
    1, 7, 9,
    1, 9, 3,
    4, 6, 7,
    4, 0, 6,
    4, 1, 0,
    4, 7, 1,
    5, 9, 8,
    5, 8, 2,
    5, 2, 3,
    5, 3, 9;

  Eigen::MatrixXd difference_positions;
  Eigen::MatrixXi difference_faces;
  Eigen::VectorXi J;

  igl::copyleft::cgal::mesh_boolean(big_positions, big_faces, small_positions, small_faces, igl::MESH_BOOLEAN_TYPE_MINUS, difference_positions, difference_faces, J);
  igl::writeOBJ("diff.obj", difference_positions, difference_faces);
  
  return 0;
}
