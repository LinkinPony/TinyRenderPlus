//
// Created by linkinpony on 23-1-7.
//
#include <iostream>
#include <memory>
#include <editor.h>
#include <scene.h>
#include <object.h>
#include <shader.h>
#include <bling_phong_shader.h>
int main() {
  {
    //init
    std::unique_ptr<Shader> shader(new BlingPhongShader());
    auto sc = std::make_unique<Scene>(1280,720,0.45,std::move(shader));
    sc->addObject(std::make_unique<Object>("models/diablo/diablo3_pose.obj"));
    auto editor = std::make_unique<Editor>();
    editor->loadScene(std::move(sc));
    editor->run();
  }
  return 0;
}