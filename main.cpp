//
// Created by linkinpony on 23-1-7.
//
#include <iostream>
#include <memory>
#include <editor.h>
#include <scene.h>
int main() {
  auto sc = std::make_unique<Scene>(500,300,0.45);
  auto editor = std::make_unique<Editor>();
  editor->loadScene(std::move(sc));
  editor->run();

  return 0;
}