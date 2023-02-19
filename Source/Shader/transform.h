//
// Created by linkinpony on 23-1-25.
//

#ifndef TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
#define TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
#include <eigen3/Eigen/Eigen>

class Transform {
 public:
  static Eigen::Matrix4f viewTrans(const Eigen::Vector3f &camera_position,
                                   const Eigen::Vector3f &camera_target,
                                   const Eigen::Vector3f &up_direction);
  static Eigen::Matrix4f orthoTrans(float l,float r,float b,float t,float f,float n);
  static Eigen::Matrix4f perspTrans(float f,float n);
  static Eigen::Matrix4f viewportTrans(int width,int height);
  static Eigen::Matrix4f modelTrans(float angle,float scale);
  static Eigen::Matrix4f projectionTrans(float eye_fov, float aspect_ratio, float zNear, float zFar);
};
#endif //TINYRENDERPLUS_SOURCE_SHADER_TRANSFORM_H_
