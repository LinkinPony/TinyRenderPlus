//
// Created by linkinpony on 23-1-20.
//

#include "bling_phong_shader.h"

#include <shader_data.h>

#include "eigen3/Eigen/Eigen"

#include <cmath>
#include <iostream>
TGAColor BlingPhongShader::diffuse(const Eigen::Vector2f &uvf, int obj_id, const ShaderUniformData &u_data) {
  auto &diffusemap = u_data.u_diffuse[obj_id];
  Eigen::Vector2f uv(uvf[0] * diffusemap->get_width(), uvf[1] * diffusemap->get_height());
  return diffusemap->get(uv[0], uv[1]);
}
Eigen::Vector3f Barycentric(const Eigen::Vector3f (&vertex)[3], const Eigen::Vector2i &P) {
  //Calculate barycentric of P in triangle defined by vertex[3]
  Eigen::Vector3f result =  //cal cross
      Eigen::Vector3f(vertex[1].x() - vertex[0].x(), vertex[2].x() - vertex[0].x(), vertex[0].x() - P.x())
          .cross(Eigen::Vector3f(vertex[1].y() - vertex[0].y(), vertex[2].y() - vertex[0].y(), vertex[0].y() - P.y()));
  if (abs(result.z()) < 1)return Eigen::Vector3f(-1, 1, 1);
  result.x() /= result.z(), result.y() /= result.z();
  return {(float) 1.0 - result.x() - result.y(), result.x(), result.y()};
}
void BlingPhongShader::fragmentShader(ShaderVaryingData &data, const ShaderUniformData &u_data) {
  int x = data.coord_x;
  int y = data.coord_y;
  Eigen::Vector3f vertex[3];
  for (int i = 0; i < 3; i++) {
    float w = data.vertex[i].w();
    vertex[i] = data.vertex[i].head<3>();
    vertex[i] /= w;
  }
  Eigen::Vector3f bary = Barycentric(vertex, Eigen::Vector2i(x, y));
  if (bary.x() < -eps || bary.y() < -eps || bary.z() < -eps) {
    data.skip = true;
    return;
  }
  data.depth = 0;
  Eigen::Vector2f diffuse_uv = Eigen::Vector2f::Zero();
  Eigen::Vector3f cur_position = Eigen::Vector3f::Zero();
  Eigen::Vector3f cur_norm = Eigen::Vector3f::Zero();
  for (int i = 0; i < 3; i++) {
    data.depth += bary[i] * vertex[i].z();
    diffuse_uv.x() += bary[i] * data.uv[i].x();
    diffuse_uv.y() += bary[i] * data.uv[i].y();
    cur_position += (vertex[i] * bary[i]);
    cur_norm = cur_norm + data.norm[i] * bary[i];
  }
  data.texture_color = diffuse(diffuse_uv, data.object_id, u_data);
  //diffuse light
  data.output_color = data.texture_color;
  float total_light = 0;
  for (auto light : u_data.lights) {
    Eigen::Vector3f light_direction = (light.light_position - cur_position).normalized();
    float dis = (light.light_position - cur_position).norm();
    float diffuse_light = light.light_intensity / (dis * dis) * fmax(0.0, light_direction.dot(cur_norm));
    //specular light
    //TODO: change this eye_pos
    auto eye_position = Eigen::Vector3f(0, 0, 0);
    Eigen::Vector3f view_direction = (eye_position - cur_position).normalized();
    Eigen::Vector3f bisector = (view_direction + light_direction).normalized();
    float specular_light = light.light_intensity / (dis * dis) * pow(fmax(0.0, cur_norm.dot(bisector)), 5);
    total_light += (diffuse_light + specular_light);
  }
  data.output_color = data.texture_color * total_light;
}
void BlingPhongShader::vertexShader(ShaderVaryingData &data, const ShaderUniformData &u_data) {
  //TODO: finish it
  for (int i = 0; i < 3; i++) {
    data.vertex[i] = u_data.camera_MVP * data.vertex[i];
  }
}
