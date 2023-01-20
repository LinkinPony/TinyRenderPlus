//
// Created by linkinpony on 23-1-20.
//

#ifndef TINYRENDERPLUS_SOURCE_SHADER_SHADER_PIPELINE_H_
#define TINYRENDERPLUS_SOURCE_SHADER_SHADER_PIPELINE_H_
#include <shader.h>
#include <shader_data.h>

#include <vector>

class ShaderPipeline {
  //TODO: this is just a sketch
 public:
  void addVaryingData(const ShaderVaryingData & v_data);
  void processAllData();
  void processSingleVertexData(const ShaderVaryingData & data);
  void processSingleFragmentData(const ShaderVaryingData & data);
 private:
  //TODO: vector will malloc and realloc memory every time. This is slow.
  std::vector<ShaderVaryingData>vertex_varying_buffer_;
  std::vector<ShaderVaryingData>fragment_varying_buffer_;
};

#endif //TINYRENDERPLUS_SOURCE_SHADER_SHADER_PIPELINE_H_
