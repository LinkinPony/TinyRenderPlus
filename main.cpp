//
// Created by linkinpony on 23-1-7.
//
#include <benchmark/benchmark.h>
#include <bling_phong_shader.h>
#include <editor.h>
#include <object.h>
#include <rasterizer.h>
#include <render.h>
#include <scene.h>
#include <shader.h>

#include <chrono>
#include <iostream>
#include <memory>
int main(int argc, char* argv[]) {
  std::shared_ptr<Shader> shader(new BlingPhongShader());
  std::shared_ptr<Render> render(new Rasterizer());
  auto sc = std::make_shared<Scene>(500, 500, 0.45f, shader, render);
  auto resource_pool = std::make_shared<TexturePool>();
  auto model_one = Model("Resource/Model/nanosuit/nanosuit.obj", resource_pool,
                         Transform::modelTrans(0, 1));
  sc->addModel(model_one);
  // sc->addObject(std::make_unique<Object>("E:/Github/temp/Model/diablo/diablo3_pose"));
  //    sc->nextFrame();
  auto editor = std::make_shared<Editor>();
  editor->loadScene(sc);
  // editor->scene_->nextFrame();
  if (argc == 1) {
    editor->run();
  }
  // editor->scene_->writeTGAImage("result.tga");
  return 0;
  // auto start = std::chrono::steady_clock::now();
  // int n = 20;
  // for(int i = 1;i <= n;i++){
  //   editor->scene_->nextFrame();
  // }
  // auto end = std::chrono::steady_clock::now();
  // std::chrono::duration<double> elapsed_seconds = end-start;
  // float total_ms = elapsed_seconds.count() * 1000;
  // std::cout << "Total time: " << total_ms << " ms.\n";
  // std::cout << "Time per frame: " << total_ms / n << " ms/f.\n";
  // std::cout << "fps: " << 1000 / (total_ms / n) << std::endl;
  return 0;
}