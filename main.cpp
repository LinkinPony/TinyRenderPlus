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

#include <vector>
std::shared_ptr<Editor> editor;
static void BM_renderFrame(benchmark::State& state) {
  std::shared_ptr<Shader> shader(new BlingPhongShader());
  std::shared_ptr<Render> render(new Rasterizer());
  auto sc = std::make_shared<Scene>(500, 500, 0.45f, shader, render);
  auto resource_pool = std::make_shared<TexturePool>();
  auto model_one = Model("Resource/Model/nanosuit/nanosuit.obj", resource_pool,
                         Transform::modelTrans(0, 1));
  sc->addModel(model_one);
  // sc->addObject(std::make_unique<Object>("E:/Github/temp/Model/diablo/diablo3_pose"));
  //    sc->nextFrame();
  editor = std::make_shared<Editor>();
  editor->loadScene(sc);
  editor->scene_->nextFrame();
  for (auto _ : state) {
    editor->scene_->nextFrame();
  }
}
// Register the function as a benchmark
//BENCHMARK(BM_renderFrame);
//BENCHMARK_MAIN();
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
  editor = std::make_shared<Editor>();
  editor->loadScene(sc);
  editor->scene_->nextFrame();
  if (argc == 1) {
    editor->run();
  }
  return 0;
}