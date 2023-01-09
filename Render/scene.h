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
class Scene {
 private:
  int width_;
  int height_;
  float scale_;
  float fov_;//vertical fov
  float aspect_ratio_;//aspect_ration = width / height

  std::unique_ptr<float[]>zbuffer_;
  std::unique_ptr<char[]>image_data_;

  GLuint render_result_;

 public:
  Scene(int width,int height,float fov)
  :width_(width),height_(height),fov_(fov){
    aspect_ratio_ = static_cast<float>(width) / height;
    zbuffer_ = std::make_unique<float[]>(width * height);
    image_data_ = std::make_unique<char[]>(width * height * 4);
  }

  bool loadTextureFromMemory();
  void genTextureInit();

  void nextFrame();
  void addObject(std::unique_ptr<Object> obj);
  int get_width();
  int get_height();
  GLuint get_render_result();
  void set_width();
  void set_height(int height);
  void set_width(int width);
};

#endif //TINYRENDERPLUS_RENDER_SCENE_H_
