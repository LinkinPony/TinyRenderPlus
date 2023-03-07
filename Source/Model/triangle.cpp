//
// Created by linkinpony on 23-1-7.
//

#include "triangle.h"
#include <mesh.h>
inline Eigen::Vector4f vec4(const Eigen::Vector3f& in) {
  return Eigen::Vector4f(in[0], in[1], in[2],1.0f);
}
Triangle::Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
  memset(color_, 0, sizeof(color_));
  v_[0] = vec4(v0.position);
  v_[1] = vec4(v1.position);
  v_[2] = vec4(v2.position);
  normal_[0] = v0.normal;
  normal_[1] = v1.normal;
  normal_[2] = v2.normal;
  texture_coords_[0] = v0.tex_coord;
  texture_coords_[1] = v1.tex_coord;
  texture_coords_[2] = v2.tex_coord;
}
