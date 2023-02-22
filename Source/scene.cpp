//
// Created by linkinpony on 23-1-8.
//
#include <scene.h>
#include <transform.h>
#include <triangle.h>

#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <sstream>
void Scene::initVertexVaryingData() {
  // TODO: change function name
  object_vertex_vary_data_.clear();
  fragment_vary_data_.clear();
  for (const auto &objs : objects_) {
    std::vector<ShaderVaryingData> temp_vec;
    temp_vec.reserve((std::size_t)objs->triangles_.size());
    for (const auto &it : objs->triangles_) {
      temp_vec.emplace_back(it);
    }
    object_vertex_vary_data_.push_back(temp_vec);
  }
}

void Scene::processSingleVertex(ShaderVaryingData &data) {
  shader_->vertexShader(data, shader_uniform_data_);
  // data.debugPrint();
  //  cut off
  bool skip_flag = true;
  for (int i = 0; i < 3; i++) {
    float absw = fabs(data.vertex[i].w());
    if (fabs(data.vertex[i].x()) < absw && fabs(data.vertex[i].y()) < absw &&
        fabs(data.vertex[i].z()) < absw) {
      skip_flag = false;
    }
  }
  // data.skip |= skip_flag;
  if (!data.skip) {
    for (int i = 0; i < 3; i++) {
      data.vertex[i] = shader_uniform_data_.m_viewport * data.vertex[i];
    }
    for (int i = 0; i < 3; i++) {
      float w = data.vertex[i].w();
      data.vertex[i].x() /= w;
      data.vertex[i].y() /= w;
      data.vertex[i].z() /= w;
    }
  }
}

void Scene::processAllVertex() {
  for (auto &vec : object_vertex_vary_data_) {
    for (auto &it : vec) {
      processSingleVertex(it);
    }
  }
}
void Scene::processSingleTriange(ShaderVaryingData &data) {
  // In previous version, we calculate all stuff under viewport translation.
  // This will cause many mystery problem.
  // Now we calculate triangles under MVP translation,
  // and pass a screen coord parameter to drawer.
  if (data.skip) {
    return;
  }
  const auto &vertex = data.vertex;  // in MVP translation.
  int width = get_width();
  int height = get_height();
  int lx =
      std::max(std::min({vertex[0].x(), vertex[1].x(), vertex[2].x()}), 0.f);
  int rx = std::min(std::max({vertex[0].x(), vertex[1].x(), vertex[2].x()}),
                    width - 1.f);
  int ly =
      std::max(std::min({vertex[0].y(), vertex[1].y(), vertex[2].y()}), 0.f);
  int ry = std::min(std::max({
                        vertex[0].y(),
                        vertex[1].y(),
                        vertex[2].y(),
                    }),
                    height - 1.f);
  for (int x = lx; x <= rx; x++) {
    for (int y = ly; y <= ry; y++) {
      auto temp_data = data;
      temp_data.coord_x = x;
      temp_data.coord_y = y;
      fragment_vary_data_.emplace_back(temp_data);
    }
  }
}
void Scene::processAllTriangle() {
  for (auto &vec : object_vertex_vary_data_) {
    for (auto &it : vec) {
      processSingleTriange(it);
    }
  }
}
void Scene::processSingleFragment(ShaderVaryingData &data) {
  shader_->fragmentShader(data, shader_uniform_data_);
}
void Scene::processAllFragment() {
  for (auto &it : fragment_vary_data_) {
    processSingleFragment(it);
  }
}
void Scene::drawSingleFragment(ShaderVaryingData &data) {
  // data.debugPrint();
  if (data.skip) {
    return;
  }
  int x = data.coord_x;
  int y = data.coord_y;
  int idx = x + y * get_width();
  if (z_buffer_[idx] > data.depth) {
    z_buffer_[idx] = data.depth;
    auto &color = data.output_color;
    // data.output_color.a = 255;
    // color = color * 10;
    render_buffer_->set(x, y, color);
    // data.debugPrint();
  }
  // TODO: delete debug output
}
void Scene::drawAllFragment() {
  for (auto &it : fragment_vary_data_) {
    drawSingleFragment(it);
  }
}
void Scene::nextFrame() {
  // TODO: lots of work to do
  applySceneConfig();
  std::fill(z_buffer_.begin(), z_buffer_.end(), INF);
  render_buffer_->clear();
  initVertexVaryingData();
  processAllVertex();
  processAllTriangle();
  processAllFragment();
  drawAllFragment();
  // render_buffer_.flip_vertically();
  // for (int i = 0; i < get_width(); i++) {
  //   for (int j = 0; j < get_height(); j++) {
  //     render_buffer_.set(i, j, TGAColor(255, 255, 255, 255));
  //   }
  // }
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
void Scene::addObject(std::shared_ptr<Object> obj) {
  objects_.emplace_back(obj);
  shader_uniform_data_.u_diffuse.push_back(obj->diffuse_map_);
}
void Scene::set_camera_mvp_matrix(const Eigen::Matrix4f &mat) {
  shader_uniform_data_.camera_MVP = mat;
}
void Scene::addLight(Light &light) {
  shader_uniform_data_.lights.emplace_back(light);
}
void Scene::applySceneConfig() {
  std::shared_ptr<Camera> camera = config_->camera_;
  //camera->moveByEulerianAngles(config_->pitch, config_->yaw);
  generateMVPMatrix();
  shader_uniform_data_.m_viewport =
      Transform::viewportTrans(config_->width, config_->height);
}

void Scene::generateMVPMatrix() {
  // Model Matrix
  // TODO: you need assign seperate model matrix for each model
  auto &camera = config_->camera_;
  auto m_model = Transform::modelTrans(0, 1);
  // View Matrix
  // Controled by camera
  auto m_view = camera->getLookAtMat();
  // Projection Matrix
  auto m_projection = Transform::projectionTrans(
      config_->fov, config_->aspect_ratio, config_->zNear, config_->zFar);
  auto matrix = m_projection * m_view * m_model;
  // TODO: delete this output
  Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
  std::cout << matrix.format(CleanFmt) << "\n------------------\n";
  set_camera_mvp_matrix(matrix);
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
  res_stream << "camera_position : " << camera_->getref_camera_position()
             << std::endl;
  res_stream << "camera_direction : " << camera_->getref_camera_direction()
             << std::endl;
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