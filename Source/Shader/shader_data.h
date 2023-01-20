//
// Created by linkinpony on 23-1-14.
//
//Use this to simulate shader's data pipline.
//Every uniform data start with u_,
//and every variable data start with v_.

#ifndef TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_
#define TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_

#include <vector>
#include <tgaimage.h>
#include <triangle.h>
#include <eigen3/Eigen/Eigen>
struct Light{
  Eigen::Vector3f light_position;
  Eigen::Vector3f eye_position;
  float light_intensity;
};

struct ShaderUniformData{
  //according to uniform in shader. should only constract once.
  std::vector<TGAImage> u_texture;//texture for each object. indexed with object id.
  std::vector<TGAImage> u_diffuse;//diffuse map for each object. indexed with object id.
};


struct ShaderVaryingData{
  //varying
  //TODO: learn and optimize with Milo Yip's flexible: https://github.com/miloyip/flexible
  //TODO: try Struct Of Array(SOA) optimize.
  int coord_x,coord_y;//screen coordinate
  int object_id;//for u_texture,u_diffuse
  std::vector<Light>lights;//TODO: maybe need optimize
  Eigen::Vector4f vertex[3];//modified in vertex shader, and should be read-only in fragment shader
  Eigen::Vector2f uv[3];//texture coord, range in [0,1]
  Eigen::Vector3f norm[3];
  Eigen::Vector3f color[3];
  Eigen::Vector2f texture_coords[3];
  float depth;//z-buffer
  TGAColor texture_color;
  TGAColor output_color;
  bool skip;//1 -> skip this pixel.
  ShaderVaryingData(const Triangle & tri){
    for(int i = 0;i < 3;i++){
      vertex[i] = tri.v_[i];
    }
    for(int i = 0;i < 3;i++){
      color[i] = tri.color_[i];
    }
    for(int i = 0;i < 3;i++){
      norm[i] = tri.normal_[i];
    }
    for(int i = 0;i < 3;i++){
      texture_coords[i] = tri.texture_coords_[i];
    }
  }
};
#endif //TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_
