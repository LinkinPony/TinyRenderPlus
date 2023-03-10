//
// Created by linkinpony on 23-1-20.
//

#include "bling_phong_shader.h"

#include <shader_data.h>

#include <cmath>
#include <iostream>

#include "eigen3/Eigen/Eigen"

const float BlingPhongShader::eps = 1e-6;

void BlingPhongShader::setUniformData(const ShaderUniformData &data) {
  uniform_data_ = data;
}

ShaderUniformData &BlingPhongShader::getref_shader_uniform_data() {
  return uniform_data_;
}

TGAColor BlingPhongShader::diffuse(const Eigen::Vector2f &uvf, int obj_id,
                                   const ShaderUniformData &u_data) {
  auto &diffusemap = u_data.u_diffuse[obj_id];
  Eigen::Vector2i uv(uvf[0] * diffusemap->get_width(),
                     uvf[1] * diffusemap->get_height());
  return diffusemap->get(uv[0], uv[1]);
}
Eigen::Vector3f Barycentric(const Eigen::Vector4f (&vertex)[3],
                            const Eigen::Vector2i &P) {
  // Calculate barycentric of P in triangle defined by vertex[3]
  Eigen::Vector3f result =  // cal cross
      Eigen::Vector3f(vertex[1][0] - vertex[0][0], vertex[2][0] - vertex[0][0],
                      vertex[0][0] - P[0])
          .cross(Eigen::Vector3f(vertex[1][1] - vertex[0][1],
                                 vertex[2][1] - vertex[0][1],
                                 vertex[0][1] - P[1]));
  if (abs(result.z()) < BlingPhongShader::eps) return Eigen::Vector3f(-1, 1, 1);
  float x = result[0];
  float y = result[1];
  float z = result[2];
  x /= z;
  y /= z;
  return {(float)1.0 - x - y, x, y};
}
void BlingPhongShader::fragmentShader(ShaderVaryingData &data) {
  if (data.skip) {
    return;
  }
  int x = data.coord_x;
  int y = data.coord_y;
  auto &vertex = data.vertex;
  data.output_color = TGAColor(255, 255, 255, 255);

  // return;
  Eigen::Vector3f bary = Barycentric(vertex, Eigen::Vector2i(x, y));
  
  if (bary.x() < -eps || bary.y() < -eps || bary.z() < -eps) {
    data.skip = true;
    return;
  }
  return;
   std::cout << x << " " << y << " | " << data.vertex[0].x() << " "
            << data.vertex[0].y() << " | " << data.vertex[1].x() << " "
            << data.vertex[1].y() << " | " << data.vertex[2].x() << " "
            << data.vertex[2].y() << " | " << bary.x() << " " << bary.y()
            << " " << bary.z() << std::endl;
  data.depth = 0;
  Eigen::Vector2f diffuse_uv = Eigen::Vector2f::Zero();
  Eigen::Vector3f cur_position = Eigen::Vector3f::Zero();
  Eigen::Vector3f cur_norm = Eigen::Vector3f::Zero();
  for (int i = 0; i < 3; i++) {
    data.depth += bary[i] * vertex[i].z();
    diffuse_uv[0] += bary[i] * data.texture_coords[i][0];
    diffuse_uv[1] += bary[i] * data.texture_coords[i][1];
    cur_position = cur_position + (vertex[i].head<3>() * bary[i]);
    cur_norm = cur_norm + data.norm[i] * bary[i];
  }
  // data.texture_color = diffuse(diffuse_uv, data.object_id, uniform_data_);
  //  diffuse light
  float total_light = 0;
  for (auto light : uniform_data_.lights) {
    float ambient_light = 1;
    Eigen::Vector3f light_direction =
        (light.light_position - cur_position).normalized();
    float dis = (light.light_position - cur_position).norm();
    float diffuse_light = light.light_intensity / (dis * dis) *
                          fmax(0.0, light_direction.dot(cur_norm));
    // specular light
    // TODO: change this eye_pos
    auto eye_position = Eigen::Vector3f(0, 0, 10);
    Eigen::Vector3f view_direction = (eye_position - cur_position).normalized();
    Eigen::Vector3f bisector = (view_direction + light_direction).normalized();
    float specular_light = light.light_intensity / (dis * dis) *
                           pow(fmax(0.0, cur_norm.dot(bisector)), 150);
    total_light += (diffuse_light + specular_light + ambient_light);
  }
  if (total_light < light_threshold) {
    data.skip = true;
    return;
  }
  // data.output_color = data.texture_color * total_light;
  data.output_color = TGAColor(255, 255, 255, 255);
  std::cout << x << " " << y << " " << data.output_color << "\n";
  //  std::cout << "total_light: " << total_light << " " << data.output_color <<
  //  "\n";
}
void BlingPhongShader::vertexShader(ShaderVaryingData &data) {
  for (int i = 0; i < 3; i++) {
    data.vertex[i] = uniform_data_.m_vertex * data.vertex[i];
  }
}
