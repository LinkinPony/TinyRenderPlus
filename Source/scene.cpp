//
// Created by linkinpony on 23-1-8.
//

#include <scene.h>
#include <triangle.h>
void Scene::initVertexVaryingData(){
  //TODO: change function name
  object_vertex_vary_data_.clear();
  fragment_vary_data_.clear();
  for(const auto& objs:objects_){
    std::vector<ShaderVaryingData> temp_vec;
    temp_vec.reserve((std::size_t)objs->triangles_.size());
    for(const auto& it:objs->triangles_){
      temp_vec.emplace_back(it);
    }
    object_vertex_vary_data_.push_back(temp_vec);
  }
}

void Scene::processSingleVertex(ShaderVaryingData &data) {
  shader_->vertexShader(data,shader_uniform_data_);
}

void Scene::processAllVertex() {
  for(auto & vec:object_vertex_vary_data_){
    for(auto & it:vec){
      processSingleVertex(it);
    }
  }
}
void Scene::processSingleTriange(ShaderVaryingData &data) {
  const auto & vertex = data.vertex;
  int width = get_width();
  int height = get_height();
  int lx = std::max(std::min({vertex[0].x(),vertex[1].x(),vertex[2].x()}),0.f);
  int rx = std::min(std::max({vertex[0].x(),vertex[1].x(),vertex[2].x()}),width-1.f);
  int ly = std::max(std::min({vertex[0].y(),vertex[1].y(),vertex[2].y()}),0.f);
  int ry = std::min(std::max({vertex[0].y(),vertex[1].y(),vertex[2].y(),}),height-1.f);
  for(int x = lx;x <= rx;x++){
    for(int y = ly;y <= ry;y++){
      auto temp_data = data;
      temp_data.coord_x = x;
      temp_data.coord_y = y;
      fragment_vary_data_.emplace_back(temp_data);
    }
  }
}
void Scene::processAllTriangle() {
  for(auto & vec:object_vertex_vary_data_){
    for(auto & it:vec){
      processSingleTriange(it);
    }
  }
}
void Scene::processSingleFragment(ShaderVaryingData &data) {
  shader_->fragmentShader(data,shader_uniform_data_);
}
void Scene::processAllFragment() {
  for(auto & it:fragment_vary_data_){
    processSingleFragment(it);
  }
}
void Scene::drawSingleFragment(ShaderVaryingData &data) {
  if(data.skip){
    return;
  }
  int x = data.coord_x;
  int y = data.coord_y;
  auto & color = data.output_color;
  render_buffer_.set(x,y,color);
//  data.debugPrint();
}
void Scene::drawAllFragment() {
  for(auto & it:fragment_vary_data_){
    drawSingleFragment(it);
  }
}
void Scene::nextFrame() {
  //TODO: lots of work to do
  std::fill(z_buffer_.begin(), z_buffer_.end(),INF);
  initVertexVaryingData();
  processAllVertex();
  processAllTriangle();
  processAllFragment();
  drawAllFragment();
}
int Scene::get_height() {
  return height_;
}
int Scene::get_width() {
  return width_;
}
void Scene::set_height(int height) {
  height_ = height;
  aspect_ratio_ = static_cast<float>(width_) / height_;
}
void Scene::set_width(int width) {
  width_ = width;
  aspect_ratio_ = static_cast<float>(width_) / height_;
}
void Scene::genTextureInit() {
  glGenTextures(1, &render_result_);
  glBindTexture(GL_TEXTURE_2D, render_result_);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

}
bool Scene::loadTextureFromMemory() {

  // Load from file
//  raw_image_width = ShaderGlobal::current_drawer->get_width();
//  raw_image_height = ShaderGlobal::current_drawer->get_height();
//  image_data = ShaderGlobal::current_drawer -> data;
  if (render_buffer_.data == nullptr)
    return false;
//  render_buffer_.flip_horizontally();
  // Create a OpenGL texture identifier
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, render_buffer_.data);
  return true;
}

GLuint Scene::get_render_result() {
  return render_result_;
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