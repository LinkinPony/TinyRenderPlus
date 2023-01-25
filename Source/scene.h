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
#include <tgaimage.h>
#include <render.h>
#include <shader.h>
#include <shader_data.h>
#include <memory>
class Scene {
 private:
  const float INF = 1e18;
 private:
  int width_;
  int height_;
  float scale_;
  float fov_;//vertical fov
  float aspect_ratio_;//aspect_ration = width / height

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
  void drawSingleFragment(const ShaderVaryingData & data);
  void drawAllFragment();
//  RenderInterface render;
 public:
  Scene(int width,int height,float fov,std::unique_ptr<Shader>shader)
  :width_(width),height_(height),fov_(fov),shader_(std::move(shader)){
    aspect_ratio_ = static_cast<float>(width) / height;
    z_buffer_.assign(width * height,INF);
    render_buffer_ = TGAImage(width,height,TGAImage::RGB);
    genTextureInit();
  }

  bool loadTextureFromMemory();
  void genTextureInit();

  void nextFrame();
  void addObject(std::shared_ptr<Object> obj);
 public:
  int get_width();
  int get_height();
  GLuint get_render_result();
  void set_height(int height);
  void set_width(int width);

};

#endif //TINYRENDERPLUS_RENDER_SCENE_H_
