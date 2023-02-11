//
// Created by linkinpony on 23-1-7.
//

#ifndef TINYRENDERPLUS__TRIANGLE_H_
#define TINYRENDERPLUS__TRIANGLE_H_
#include <eigen3/Eigen/Eigen>
class Triangle {
 public:
  Eigen::Vector3f v_[3];//vertex, (x,y,z)
  Eigen::Vector3f color_[3];
  Eigen::Vector3f normal_[3];
  Eigen::Vector2f texture_coords_[3];

  int material_id_;//TODO: this may be useless

};

#endif //TINYRENDERPLUS__TRIANGLE_H_
