//
// Created by linkinpony on 23-1-8.
//
#ifndef TINYRENDERPLUS_GUI_EDITOR_H_
#define TINYRENDERPLUS_GUI_EDITOR_H_
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

#include <memory>
#include "scene.h"
//import Render.Scene;

class Editor {
 public:
  std::shared_ptr<Scene>scene_;
  GLuint raw_image_texture_;
 public:
  void loadScene(std::shared_ptr<Scene> scene);
  int run();
};

#endif //TINYRENDERPLUS_GUI_EDITOR_H_
