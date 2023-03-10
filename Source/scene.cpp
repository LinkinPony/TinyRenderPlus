//
// Created by linkinpony on 23-1-8.
//
#include <scene.h>
#include <transform.h>
#include <triangle.h>

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <sstream>
std::string Scene::getMatrixInfo() {
  Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
  std::stringstream result;
  result << shader_->getref_shader_uniform_data().m_vertex.format(CleanFmt);
  return result.str();
}
void Scene::nextFrame() {
  assert(render_ != nullptr, "render_ is null.");
  render_->applySceneConfig(config_, shader_);
  render_->newFrame(); 
  for (auto &it_model : model_) {
    // do something like configue shader
    it_model.draw(render_);
  }
  // TODO: lots of work to do
  // applySceneConfig();
  // std::fill(z_buffer_.begin(), z_buffer_.end(), INF);
  // render_buffer_->clear();
  // initVertexVaryingData();
  // shader_->setUniformData(shader_uniform_data_);
  // processAllVertex();
  // processAllTriangle();
  // processAllFragment();
  // drawAllFragment();
}
int Scene::get_height() { return config_->height; }
int Scene::get_width() { return config_->width; }
void Scene::set_height(int height) {
  config_->height = height;
  config_->aspect_ratio = static_cast<float>(config_->width) / config_->height;
}
void Scene::set_width(int width) {
  config_->width = width;
  config_->aspect_ratio = static_cast<float>(config_->width) / config_->height;
}

void Scene::writeTGAImage(const std::string &filename) {
  this->render_buffer_->write_tga_file(filename.c_str());
}

std::string SceneConfig::getInfoString() {
  std::stringstream res_stream;
  res_stream << " ---- config ----\n";
  res_stream << "width x height = " << width << " x " << height << std::endl;
  res_stream << "scale = " << scale << std::endl;
  res_stream << "fov = " << fov << std::endl;
  res_stream << "aspect_ratio = " << aspect_ratio << std::endl;
  res_stream << "zNear = " << zNear << std::endl;
  res_stream << "zFar = " << zFar << std::endl;
  const auto cam_pos = camera_->getref_camera_position();
  const auto cam_dir = camera_->getref_camera_direction();
  res_stream << "camera_position : [" << cam_pos.x() << "," << cam_pos.y()
             << "," << cam_pos.z() << "]" << std::endl;
  res_stream << "camera_direction : [" << cam_dir.x() << "," << cam_dir.y()
             << "," << cam_dir.z() << "]" << std::endl;
  res_stream << " ---- ------ ----\n";
  return res_stream.str();
}

void SceneConfig::debugPrint() {
  std::cerr << " ---- config ----\n";
  std::cerr << "width x height = " << width << " x " << height << std::endl;
  std::cerr << "scale = " << scale << std::endl;
  std::cerr << "fov = " << fov << std::endl;
  std::cerr << "aspect_ratio = " << aspect_ratio << std::endl;
  std::cerr << "zNear = " << zNear << std::endl;
  std::cerr << "zFar = " << zFar << std::endl;
  std::cerr << "camera_position : " << camera_->getref_camera_position()
            << std::endl;
  std::cerr << "camera_direction : " << camera_->getref_camera_direction()
            << std::endl;
  std::cerr << " ---- ------ ----\n";
}