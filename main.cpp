//
// Created by linkinpony on 23-1-7.
//
#include <iostream>
#include <memory>
#include <chrono>

#include <editor.h>
#include <scene.h>
#include <object.h>
#include <shader.h>
#include <bling_phong_shader.h>

#include <benchmark/benchmark.h>
int main(int argc,char * argv[]) {
  std::unique_ptr<Shader> shader(new BlingPhongShader());
  auto sc = std::make_unique<Scene>(500,500,0.45,std::move(shader));
  sc->addObject(std::make_unique<Object>("/home/linkinpony/temp/models/diablo/diablo3_pose"));
//    sc->nextFrame();
  auto editor = std::make_shared<Editor>();
  editor->loadScene(std::move(sc));

  if(argc > 1){
    editor->run();
  }
  return 0;
  auto start = std::chrono::steady_clock::now();
  int n = 20;
  for(int i = 1;i <= n;i++){
    editor->scene_->nextFrame();
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  float total_ms = elapsed_seconds.count() * 1000;
  std::cout << "Total time: " << total_ms << " ms.\n";
  std::cout << "Time per frame: " << total_ms / n << " ms/f.\n";
  std::cout << "fps: " << 1000 / (total_ms / n) << std::endl;
  return 0;
}