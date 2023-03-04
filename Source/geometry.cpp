#include "geometry.h"
//TODO: write test
Eigen::Vector4f Geometry::lerpPoint4f(const Eigen::Vector4f& P1,
                                      const Eigen::Vector4f& P2, float x) {
  return (1 - x) * P1 + x * P2;
}

float Geometry::distancePlaneAndPoint(const Eigen::Vector4f& plane_norm,
                                      const Eigen::Vector4f& point) {
  return plane_norm.dot(point);
}

bool Geometry::isPointInsidePlane(const Eigen::Vector4f& plane_norm,
                                  const Eigen::Vector4f& point) {
  // plane: Ax + By + Cz + D = 0
  // plane_norm = (A,B,C,1), and D = 1
  // if point is inside the plane,there must be Ax / w + By / w + Cz / w + D >
  // O so we can judge Ax + By + Cz + D * w > 0, which is
  //  plane_norm.dot(point)
  return distancePlaneAndPoint(plane_norm, point) > 0;
}
Eigen::Vector4f Geometry::intersectPlaneAndPoint(
    const Eigen::Vector4f& plane_norm, const Eigen::Vector4f& P1,
    const Eigen::Vector4f& P2) {
  float d1 = distancePlaneAndPoint(plane_norm, P1);
  float d2 = distancePlaneAndPoint(plane_norm, P2);
  float x = d1 / (d1 - d2);
  return lerpPoint4f(P1, P2, x);
}

std::vector<Eigen::Vector4f> Geometry::cullingByPlane(
    const std::vector<Eigen::Vector4f>& input,
    const std::vector<Eigen::Vector4f>& plane) {
    //use Sutherland-Hodgeman alogrithm
  std::vector<Eigen::Vector4f> result = input;
  for (size_t i = 0; i < plane.size(); i++) {
    auto temp(result);
    result.clear();
    size_t siz = temp.size();
    for (size_t j = 0; j < siz; j++) {
      auto cur = temp[j];
      auto pre = temp[(j + siz - 1) % siz];
      if (isPointInsidePlane(plane[i], cur)) {
        if (!isPointInsidePlane(plane[i], pre)) {
          auto inter = intersectPlaneAndPoint(plane[i], pre, cur);
          result.push_back(inter);
        }
        result.push_back(cur);
      } else if (isPointInsidePlane(plane[i],pre)) {
        auto inter = intersectPlaneAndPoint(plane[i], pre, cur);
        result.push_back(inter);
      }
    }
  }
  return result;
}

