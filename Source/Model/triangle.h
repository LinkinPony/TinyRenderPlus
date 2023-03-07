#ifndef TINYRENDERPLUS__TRIANGLE_H_
#define TINYRENDERPLUS__TRIANGLE_H_

#include <eigen3/Eigen/Eigen>
class Vertex;
class Triangle {
 public:
  Eigen::Vector4f v_[3];  // vertex, (x,y,z,w = 1)
  Eigen::Vector3f color_[3];
  Eigen::Vector3f normal_[3];
  Eigen::Vector2f texture_coords_[3];

  int material_id_;  // TODO: this may be useless
  Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
  Triangle() = default;
};

#endif  // TINYRENDERPLUS__TRIANGLE_H_
