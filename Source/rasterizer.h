//
// Created by linkinpony on 23-1-14.
//

#ifndef TINYRENDERPLUS_RENDER_RASTERIZER_H_
#define TINYRENDERPLUS_RENDER_RASTERIZER_H_

#include <shader.h>
#include <shader_data.h>

#include <GLES2/gl2.h>

#include <memory>
#include <array>
//TODO: unused class, delete it.
class Rasterizer {
 public:
  GLuint renderFrame();//render single frame
 private:
  void drawTriangle(int face_index);//draw triangle by face_index. should init shaderdata first.
 private:
  std::unique_ptr<Shader>shader_;
  //TODO: vector seems a little bit slow, need profiling
  std::vector<float> z_buffer_;
  std::vector<ShaderVaryingData>shader_vary_data_;
  ShaderUniformData shader_uni_data_;
};

#endif //TINYRENDERPLUS_RENDER_RASTERIZER_H_
