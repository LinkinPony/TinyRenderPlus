#ifndef TINYRENDERPLUS_RENDER_GEOMETRY_H_
#define TINYRENDERPLUS_RENDER_GEOMETRY_H_
#include <eigen3/Eigen/Eigen>
#include <vector>
class Geometry {
 public:
  //only use these in clip coordinate system
  static Eigen::Vector4f lerpPoint4f(const Eigen::Vector4f& P1,
                                     const Eigen::Vector4f& P2, float x);
  static float distancePlaneAndPoint(const Eigen::Vector4f& plane_norm,
                                     const Eigen::Vector4f& point);
  static bool isPointInsidePlane(const Eigen::Vector4f& plane_norm,
                                 const Eigen::Vector4f& point);
  static Eigen::Vector4f intersectPlaneAndPoint(
      const Eigen::Vector4f& plane_norm, const Eigen::Vector4f& P1,
      const Eigen::Vector4f& P2);
  static std::vector<Eigen::Vector4f> cullingByPlane(
      const std::vector<Eigen::Vector4f>& input,
      const std::vector<Eigen::Vector4f>& plane);
  ;
};
#endif TINYRENDERPLUS_RENDER_GEOMETRY_H_