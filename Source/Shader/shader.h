//
// Created by linkinpony on 23-1-14.
//

#ifndef TINYRENDERPLUS_RENDER_SHADER_SHADER_H_
#define TINYRENDERPLUS_RENDER_SHADER_SHADER_H_
//abstract class of shader.
#include <shader_data.h>
class Shader {
 public:
  virtual ~Shader() = 0;
  
  // fragment shader process each pixel in screen.
  virtual void fragmentShader(ShaderVaryingData & data) = 0;
  // vertex shader apply matrix trans for each point of model.
  virtual void vertexShader(ShaderVaryingData & data) = 0;
  virtual void setUniformData(const ShaderUniformData& data) = 0;
  virtual ShaderUniformData& getref_shader_uniform_data() = 0;
};

#endif //TINYRENDERPLUS_RENDER_SHADER_SHADER_H_
