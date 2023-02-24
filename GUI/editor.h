//
// Created by linkinpony on 23-1-8.
//
#ifndef TINYRENDERPLUS_GUI_EDITOR_H_
#define TINYRENDERPLUS_GUI_EDITOR_H_
#include <memory>
#include "scene.h"
//import Render.Scene;

class Editor : public std::enable_shared_from_this<Editor> {
 public:
  std::shared_ptr<Scene>scene_;
  GLuint raw_image_texture_;
  bool capture_cursor_flag_ = false;
 public:
  
  void loadScene(std::shared_ptr<Scene> scene);
  int run();

  private:
  GLuint render_result;
   int screen_width_ = 1280;
  int screen_height_ = 720;
   

 private:
   std::shared_ptr<SceneConfig> config_;
 private:
  void buildRenderResultWidget();
  void buildConfigWidget();

 public:
  void mouseCallback(double xpos, double ypos);
  void scrollCallback(double xoffset,double yoffset);
};

#endif //TINYRENDERPLUS_GUI_EDITOR_H_
