#include <rasterizer.h>

void Rasterizer::newFrame() {
  // TODO: check if there's memory leak
  render_buffer_ = std::make_shared<TGAImage>(width_, height_, TGAImage::RGB);
  z_buffer_ = std::vector<float>(width_ * height_, INF);
}

std::shared_ptr<TGAImage> Rasterizer::getRenderResult() {
  return render_buffer_;
}

void Rasterizer::drawMesh(const Mesh &mesh) {
  cleanUpMesh();
  auto &vertices = mesh.get_vertices();
  auto &indices = mesh.get_indices();
  auto &textures = mesh.get_textures();
  //std::cout << indices.size() << std::endl;
  assert(shader_.get() != nullptr, "shader_ is null");
  assert(render_buffer_.get() != nullptr, "render_buffer_ is null");
  shader_->getref_shader_uniform_data().m_model = mesh.get_m_model();
  shader_->getref_shader_uniform_data().generateVertexMatrix();
  //shader_->getref_shader_uniform_data().debugPrint();
  //std::cout << shader_->getref_shader_uniform_data().m_vertex << std::endl;
  //assemble triangle
   for (size_t i = 0; i < indices.size(); i += 3) {
    Triangle tri(vertices[i], vertices[i + 1], vertices[i + 2]);
    vertex_vary_data_.emplace_back(tri);
  }
   processAllVertex();
   processAllTriangle();
   processAllFragment();
   drawAllFragment();
}

void Rasterizer::applySceneConfig(std::shared_ptr<SceneConfig> config,
                                  std::shared_ptr<Shader> shader) {
  // model shader
  width_ = config->width;
  height_ = config->height;
  auto &camera = config->camera_;
  shader_ = shader;
  ShaderUniformData &model_uniform = shader_->getref_shader_uniform_data();
  model_uniform.m_view = camera->getLookAtMat();
  model_uniform.m_projection = Transform::projectionTrans(
      config->fov, config->aspect_ratio, config->zNear, config->zFar);
  model_uniform.m_viewport = Transform::viewportTrans(width_, height_);
}

Rasterizer::Rasterizer() {
  render_buffer_ = nullptr;
  shader_ = nullptr;
}

void Rasterizer::cleanUpMesh() {
  fragment_vary_data_.clear();
  culled_vertex_vary_data_.clear();
  vertex_vary_data_.clear();
}

void Rasterizer::processSingleVertex(ShaderVaryingData &data) {
  shader_->vertexShader(data);
  // data.debugPrint();
  //  cut off
  bool skip_flag = false;
  int out_of_range_cnt = 0;
  for (int i = 0; i < 3; i++) {
    float absw = fabs(data.vertex[i].w());
    if (absw < keps) {
      skip_flag = true;
    }
    if (fabs(data.vertex[i].x()) > absw || fabs(data.vertex[i].y()) > absw) {
      out_of_range_cnt++;
    }
  }
  if (out_of_range_cnt == 3) skip_flag = true;
  data.skip |= skip_flag;
  if (!data.skip) {
    for (int i = 0; i < 3; i++) {
      data.vertex[i] =
          shader_->getref_shader_uniform_data().m_viewport * data.vertex[i];
    }
    for (int i = 0; i < 3; i++) {
      float w = data.vertex[i].w();
      data.vertex[i].x() /= w;
      data.vertex[i].y() /= w;
      data.vertex[i].z() /= w;
    }
  }
  if (!data.skip) {
    // TODO: there are too many stupid copy, change them by move
    culled_vertex_vary_data_.push_back(data);
  }
}

void Rasterizer::processAllVertex() {
  std::cerr << "vertex size: " << vertex_vary_data_.size() << std::endl;
  for (auto &it : vertex_vary_data_) {
    processSingleVertex(it);
  }
}
void Rasterizer::processSingleTriange(ShaderVaryingData &data) {
  const auto &vertex = data.vertex;  // in MVP translation.
  int width = width_;
  int height = height_;
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
  //std::cout << lx << " " << rx << " | " << ly << " " << ry << std::endl;
  for (int x = lx; x <= rx; x++) {
    for (int y = ly; y <= ry; y++) {
      auto temp_data = data;
      temp_data.coord_x = x;
      temp_data.coord_y = y;
      fragment_vary_data_.emplace_back(temp_data);
    }
  }
}
void Rasterizer::processAllTriangle() {
  std::cerr << "triangle size: " << culled_vertex_vary_data_.size() << std::endl;
  for (auto &it : culled_vertex_vary_data_) {
    processSingleTriange(it);
  }
}
void Rasterizer::processSingleFragment(ShaderVaryingData &data) {
  shader_->fragmentShader(data);
}
void Rasterizer::processAllFragment() {
  std::cerr << "fragment size: " << fragment_vary_data_.size() << std::endl;
  for (auto &it : fragment_vary_data_) {
    processSingleFragment(it);
  }
}
void Rasterizer::drawSingleFragment(ShaderVaryingData &data) {
  // data.debugPrint();
  if (data.skip) {
    //return;
  }
  int x = data.coord_x;
  int y = data.coord_y;
  int idx = x + y * width_;
  // TODO: move z-buffer test earlier
  if (z_buffer_[idx] > data.depth) {
    z_buffer_[idx] = data.depth;
    auto &color = data.output_color;
    // data.output_color.a = 255;
    // color = color * 10;
    render_buffer_->set(x, y, color);
    render_buffer_->set(x, y, TGAColor(255,255,255,255));
    //std::cout << x << " " << y << std::endl;
    // data.debugPrint();
  }
  // TODO: delete debug output
}
void Rasterizer::drawAllFragment() {
  for (auto &it : fragment_vary_data_) {
    drawSingleFragment(it);
  }
}