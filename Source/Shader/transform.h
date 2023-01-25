//
// Created by linkinpony on 23-1-25.
//

#ifndef TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
#define TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
#include <eigen3/Eigen/Eigen>

class Transform {
 public:
  static Eigen::Matrix4f identityMatrix4();
  static Eigen::Matrix3f identityMatrix3();
  static Eigen::Matrix4f viewTrans(const Eigen::Vector3f & e,
                                   const Eigen::Vector3f & c,
                                   const Eigen::Vector3f & t);
  static Eigen::Matrix4f orthoTrans(float l,float r,float b,float t,float f,float n);
  static Eigen::Matrix4f perspTrans(float l,float r,float b,float t,float f,float n);
  static Eigen::Matrix4f viewportTrans(int width,int height);
  static Eigen::Matrix4f getMVPMatrix(const Eigen::Vector3f & camera_position,
                                      const Eigen::Vector3f & view_center,
                                      int image_width,
                                      int image_height);
};
#endif //TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
