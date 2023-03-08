//
// Created by linkinpony on 23-1-8.
//
#ifndef TINYRENDERPLUS_RENDER_SCENE_H_
#define TINYRENDERPLUS_RENDER_SCENE_H_
#include <camera.h>
#include <model.h>
#include <object.h>
#include <render.h>
#include <scene_config.h>
#include <shader.h>
#include <shader_data.h>
#include <tgaimage.h>

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <memory>
class Scene {
 private:
  const float INF = 1e18;
  const float keps = 1e-6;

 private:
  std::shared_ptr<Render> render_;

 private:
  std::shared_ptr<SceneConfig> config_;
  std::shared_ptr<TGAImage> render_buffer_;

 private:
  std::shared_ptr<Shader> shader_;
  // TODO: vector seems a little bit slow, need profiling
  std::vector<float> z_buffer_;
  std::vector<ShaderVaryingData> fragment_vary_data_;
  std::vector<ShaderVaryingData> culled_vertex_data_;
  // todo: use point. avoid copy
  ShaderUniformData shader_uniform_data_;
  std::vector<std::shared_ptr<Object>> objects_;
  std::vector<std::vector<ShaderVaryingData>> object_vertex_vary_data_;

 private:
  std::vector<Model> model_;

 private:
  // draw triangle for specific object and it's face.

  void initVertexVaryingData();
  void processSingleVertex(ShaderVaryingData &data);
  void processAllVertex();
  void processSingleTriange(ShaderVaryingData &data);
  void processAllTriangle();
  void processSingleFragment(ShaderVaryingData &data);
  void processAllFragment();
  void drawSingleFragment(ShaderVaryingData &data);
  void drawAllFragment();

  //  RenderInterface render;
 public:
  Scene(int width, int height, float fov, std::shared_ptr<Shader> shader,
        std::shared_ptr<Render> render)
      : shader_(shader), render_(render) {
    config_ = std::make_unique<SceneConfig>(width, height, fov);
    config_->camera_ = std::make_unique<Camera>();
    z_buffer_.assign(width * height, INF);
    render_buffer_ = std::make_shared<TGAImage>(width, height, TGAImage::RGB);
  }
  void nextFrame();
  void addObject(std::shared_ptr<Object> obj);
  void addLight(Light &light);
  void applySceneConfig();  // call this before each frame rendering
  void generateMVPMatrix();
  void writeTGAImage(const std::string &filename);
  std::string getMatrixInfo();

 public:
  void addModel(const Model &model) { model_.emplace_back(model); }

 public:
  int get_width();
  int get_height();
  void set_height(int height);
  void set_width(int width);
  void set_camera_mvp_matrix(const Eigen::Matrix4f &mat);
  std::shared_ptr<SceneConfig> get_config() { return config_; }
  std::shared_ptr<TGAImage> get_render_buffer() {
    return render_->getRenderResult();
  }
};

#endif  // TINYRENDERPLUS_RENDER_SCENE_H_
