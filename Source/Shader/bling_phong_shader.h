//
// Created by linkinpony on 23-1-20.
//

#ifndef TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
#define TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
#include <shader.h>
class BlingPhongShader : public Shader {
 public:
  ~BlingPhongShader() override = default;
  void fragmentShader(ShaderVaryingData &data) override;
  void vertexShader(ShaderVaryingData &data) override;
  void setUniformData(const ShaderUniformData &data) override;
  ShaderUniformData &getref_shader_uniform_data() override;

 public:
  const static float eps;

 private:
  const float light_threshold = 1e-3;
  ShaderUniformData uniform_data_;

 private:
  TGAColor diffuse(const Eigen::Vector2f &uvf, int obj_id,
                   const ShaderUniformData &u_data);
};

#endif  // TINYRENDERPLUS_SOURCE_SHADER_BLING_PHONG_SHADER_H_
