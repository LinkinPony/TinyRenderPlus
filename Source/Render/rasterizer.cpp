#include <rasterizer.h>

void Rasterizer::newFrame() {
  // TODO: change function name
  render_buffer_->clear();
  z_buffer_.clear();
}

std::shared_ptr<TGAImage> Rasterizer::getRenderResult() {
  return std::shared_ptr<TGAImage>();
}

void Rasterizer::drawMesh(const Mesh &mesh) {}

void Rasterizer::applySceneConfig(std::shared_ptr<SceneConfig> config,
                                  std::shared_ptr<Shader> shader) {}

void Rasterizer::cleanUpMesh() {
  fragment_vary_data_.clear();
  culled_vertex_vary_data_.clear();
  vertex_vary_data_.clear();
}

void Rasterizer::processSingleVertex(ShaderVaryingData &data) {
  shader_->vertexShader(data, shader_uniform_data_);
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
      data.vertex[i] = shader_uniform_data_.m_viewport * data.vertex[i];
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
  for (auto &it : vertex_vary_data_) {
    processSingleVertex(it);
  }
}
void Rasterizer::processSingleTriange(ShaderVaryingData &data) {
  const auto &vertex = data.vertex;  // in MVP translation.
  int width = config_->width;
  int height = config_->height;
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
void Rasterizer::processAllTriangle() {
  for (auto &it : culled_vertex_vary_data_) {
    processSingleTriange(it);
  }
}
void Rasterizer::processSingleFragment(ShaderVaryingData &data) {
  shader_->fragmentShader(data, shader_uniform_data_);
}
void Rasterizer::processAllFragment() {
  for (auto &it : fragment_vary_data_) {
    processSingleFragment(it);
  }
}
void Rasterizer::drawSingleFragment(ShaderVaryingData &data) {
  // data.debugPrint();
  if (data.skip) {
    return;
  }
  int x = data.coord_x;
  int y = data.coord_y;
  int idx = x + y * config_->width;
  // TODO: move z-buffer test earlier
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
void Rasterizer::drawAllFragment() {
  for (auto &it : fragment_vary_data_) {
    drawSingleFragment(it);
  }
}