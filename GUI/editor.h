//
// Created by linkinpony on 23-1-8.
//
#ifndef TINYRENDERPLUS_GUI_EDITOR_H_
#define TINYRENDERPLUS_GUI_EDITOR_H_
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

  private:
  GLuint render_result;
 private:
  void buildRenderResultWidget();
  void buildConfigWidget();
};

#endif //TINYRENDERPLUS_GUI_EDITOR_H_
