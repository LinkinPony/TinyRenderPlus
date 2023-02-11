//
// Created by linkinpony on 23-1-8.
//
#ifndef TINYRENDERPLUS_RENDER_SCENE_H_
#define TINYRENDERPLUS_RENDER_SCENE_H_
#include <object.h>
#include <memory>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <tgaimage.h>
#include <render.h>
#include <shader.h>
#include <shader_data.h>

struct SceneConfig{
  //IMPORTANT: set default value
  //Scene
  int width = 0;
  int height = 0;
  float scale = 0;
  float fov = 0;//vertical fov
  float aspect_ratio = 0;//aspect_ration = width / height
  float zNear = 10;
  float zFar = 300;
  SceneConfig(int width,int height,float fov)
  :width(width),height(height),fov(fov){
      aspect_ratio = static_cast<float>(width) / height;
  }
  //Camera
  Eigen::Vector3f camera_position = Eigen::Vector3f::Zero();
  Eigen::Vector3f view_center = Eigen::Vector3f::Zero();
  //Light
  //TODO: maybe you can move lights and other TGAImage to resource pack.
  //TODO: use multi lights.
  Eigen::Vector3f light_position = Eigen::Vector3f::Zero();
  float light_intensity = 0;

  [[maybe_unused]] void debugPrint(){
    #ifdef NDEBUG
        return;
    #endif
    std::cerr << " ---- config ----\n";
    std::cerr << "width x height = " << width << " x " << height << std::endl;
    std::cerr << "scale = " << scale << std::endl;
    std::cerr << "fov = " << fov << std::endl;
    std::cerr << "aspect_ratio = " << aspect_ratio << std::endl;
    std::cerr << "zNear = " << zNear << std::endl;
    std::cerr << "zFar = " << zFar << std::endl;
    std::cerr << "camera_position : " << camera_position << std::endl;
    std::cerr << "view_center : " << view_center << std::endl;
    std::cerr << " ---- ------ ----\n";
  }
};
class Scene {
 private:
  const float INF = 1e18;
 private:
  std::shared_ptr<SceneConfig>config_;
  GLuint render_result_;
  TGAImage render_buffer_;
 private:
  std::unique_ptr<Shader>shader_;
  //TODO: vector seems a little bit slow, need profiling
  std::vector<float> z_buffer_;
  std::vector<ShaderVaryingData>fragment_vary_data_;
  //todo: use point. avoid copy
  ShaderUniformData shader_uniform_data_;
  std::vector<std::shared_ptr<Object>> objects_;
  std::vector<std::vector<ShaderVaryingData> > object_vertex_vary_data_;
 private:
  //draw triangle for specific object and it's face.

  void initVertexVaryingData();
  void processSingleVertex(ShaderVaryingData & data);
  void processAllVertex();
  void processSingleTriange(ShaderVaryingData & data);
  void processAllTriangle();
  void processSingleFragment(ShaderVaryingData & data);
  void processAllFragment();
  void drawSingleFragment(ShaderVaryingData & data);
  void drawAllFragment();
//  RenderInterface render;
 public:
  Scene(int width,int height,float fov,std::unique_ptr<Shader>shader)
  :shader_(std::move(shader)){
    config_ = std::make_unique<SceneConfig>(width,height,fov);
    z_buffer_.assign(width * height,INF);
    render_buffer_ = TGAImage(width,height,TGAImage::RGB);
    genTextureInit();
  }

  bool loadTextureFromMemory();
  void genTextureInit();
  void nextFrame();
  void addObject(std::shared_ptr<Object> obj);
  void addLight(Light &light);
  void applySceneConfig();//call this before each frame rendering
 public:
  int get_width();
  int get_height();
  GLuint get_render_result();
  void set_height(int height);
  void set_width(int width);
  void set_camera_mvp_matrix(const Eigen::Matrix4f &mat);
  std::shared_ptr<SceneConfig> get_config(){
    return config_;
  }
};

#endif //TINYRENDERPLUS_RENDER_SCENE_H_
