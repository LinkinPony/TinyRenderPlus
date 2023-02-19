//
// Created by linkinpony on 23-1-14.
//
//Use this to simulate shader's data pipline.
//Every uniform data start with u_,
//and every variable data start with v_.

#ifndef TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_
#define TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_

#include <utility>
#include <vector>
#include <memory>
#include <tgaimage.h>
#include <triangle.h>
#include <eigen3/Eigen/Eigen>
#include <iostream>
struct Light{
  Eigen::Vector3f light_position;
  float light_intensity;
  Light(
      Eigen::Vector3f light_position,
      float light_intensity)
      :light_position(std::move(light_position)),
       light_intensity(light_intensity){
  }
};

struct ShaderUniformData{
  //according to uniform in shader. should only constract once.
  std::vector<std::shared_ptr<TGAImage> > u_texture;//texture for each object. indexed with object id.
  std::vector<std::shared_ptr<TGAImage> > u_diffuse;//diffuse map for each object. indexed with object id.
  std::vector<std::shared_ptr<TGAImage> > u_normal_map;//normal map for each object. indexed with object id.
  Eigen::Matrix4f camera_MVP;//TODO: change this name
  std::vector<Light>lights;//TODO: maybe need optimize
};


struct ShaderVaryingData{
  //varying
  //TODO: learn and optimize with Milo Yip's flexible: https://github.com/miloyip/flexible
  //TODO: try Struct Of Array(SOA) optimize.
  int coord_x = 0,coord_y = 0;//screen coordinate
  int object_id = 0;//for u_texture,u_diffuse
  Eigen::Vector3f vertex[3];//modified in vertex shader, and should be read-only in fragment shader
  Eigen::Vector3f norm[3];
  Eigen::Vector3f color[3];
  Eigen::Vector2f texture_coords[3];//texture coord, range in [0,1]
  float depth = 0;//z-buffer
  TGAColor texture_color = TGAColor(0,0,0,0);
  TGAColor output_color = TGAColor(0,0,0,0);
  bool skip = false;//1 -> skip this pixel.
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
  [[maybe_unused]] void debugPrint(){
    #ifdef NDEBUG
        return;
    #endif
    std::cout << " ---- \n";
    std::cout << "Skipped: " << (skip?"true":"false") << std::endl;
    std::cout << "[x,y] = " << coord_x << "," << coord_y << std::endl;
    std::cout << "Object id: " << object_id << std::endl;
    std::cout << "Color: " << this->texture_color << std::endl;
    std::cout << "depth: " << this->depth << std::endl;
  }
};
#endif //TINYRENDERPLUS_RENDER_SHADER_SHADER_DATA_H_
