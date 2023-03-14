#ifndef TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_
#define TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_
#include <render.h>
#include <object.h>

#include <memory>
#include <camera.h>
#include <shader.h>
#include <shader_data.h>
#include <tgaimage.h>
#include <thread_pool.h>
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <thread>
class Rasterizer : public Render {
 public:
  void newFrame() override;
  std::shared_ptr<TGAImage> getRenderResult() override;
  void drawMesh(const Mesh& mesh) override;
  void applySceneConfig(std::shared_ptr<SceneConfig> config,std::shared_ptr<Shader> shader) override;
  Rasterizer();
  ~Rasterizer() override = default;
  private:
 private:
  const float INF = 1e9;
  const float keps = 1e-6;

  const static int kThreadNum = 8;
  int width_;
  int height_;

 private:
  ThreadPool::ThreadPool thread_pool_;
  std::shared_ptr<TGAImage> render_buffer_;
  std::shared_ptr<Shader> shader_;
 private:
  // TODO: vector seems a little bit slow, need profiling
  std::vector<float> z_buffer_;
  std::vector<ShaderVaryingData> fragment_vary_data_[kThreadNum];
  std::vector<ShaderVaryingData> culled_vertex_vary_data_;
  // todo: use point. avoid copy
  std::vector<ShaderVaryingData> vertex_vary_data_;

 private:
  // draw triangle for specific object and it's face.
  void cleanUpMesh();
  void processSingleVertex(ShaderVaryingData &data);
  void processAllVertex();
  void processSingleTriange(ShaderVaryingData &data);
  void processAllTriangle();
  void processSingleFragment(ShaderVaryingData &data);
  void processFragmentBlock(std::vector<ShaderVaryingData> &vec_data);
  void processAllFragment();
  void drawSingleFragment(ShaderVaryingData &data);
  //void drawAllFragment();
};

#endif //TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_

