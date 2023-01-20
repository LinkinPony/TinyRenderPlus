//
// Created by linkinpony on 23-1-20.
//

#ifndef TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
#define TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
#include <shader.h>
class BlingPhongShader : Shader {

 public:
  ~BlingPhongShader() override = default;
  void fragmentShader(ShaderVaryingData &data,const ShaderUniformData & u_data) override;
  void vertexShader(ShaderVaryingData &data,const ShaderUniformData & u_data) override;
 private:
  const float eps = 1e-6;
 private:
  //TODO: avoid copy
  ShaderUniformData shader_uniform_data;
  TGAColor diffuse(const Eigen::Vector2f & uvf,int obj_id);
};

#endif //TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
