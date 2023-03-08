#ifndef TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_
#define TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_
#include <render.h>
#include <object.h>

#include <memory>
#include <camera.h>
#include <render.h>
#include <shader.h>
#include <shader_data.h>
#include <tgaimage.h>

#include <eigen3/Eigen/Eigen>
#include <iostream>
class Rasterizer : public Render {
 public:
  void newFrame();
  std::shared_ptr<TGAImage> getRenderResult();
  void drawMesh(const Mesh& mesh);
  void applySceneConfig(std::shared_ptr<SceneConfig> config,std::shared_ptr<Shader> shader);
  private:
 private:
  const float INF = 1e18;
  const float keps = 1e-6;

 private:
  std::shared_ptr<TGAImage> render_buffer_;
  std::shared_ptr<Shader> shader_;
 private:
  // TODO: vector seems a little bit slow, need profiling
  std::vector<float> z_buffer_;
  std::vector<ShaderVaryingData> fragment_vary_data_;
  std::vector<ShaderVaryingData> culled_vertex_vary_data_;
  // todo: use point. avoid copy
  ShaderUniformData shader_uniform_data_;
  std::vector<ShaderVaryingData> vertex_vary_data_;

 private:
  // draw triangle for specific object and it's face.
  void cleanUpMesh();
  void processSingleVertex(ShaderVaryingData &data);
  void processAllVertex();
  void processSingleTriange(ShaderVaryingData &data);
  void processAllTriangle();
  void processSingleFragment(ShaderVaryingData &data);
  void processAllFragment();
  void drawSingleFragment(ShaderVaryingData &data);
  void drawAllFragment();
};

#endif //TINYRENDERPLUS_RENDER_RENDER_RASTERIZER_H_

