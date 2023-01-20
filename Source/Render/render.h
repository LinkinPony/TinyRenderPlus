//
// Created by linkinpony on 23-1-14.
//

#ifndef TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
#define TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
//interface of render. could be rasterizer or ray tracing or something interesting.
//TODO: this interface may be useless
#include <GLES2/gl2.h>
class RenderInterface{
 public:
  virtual GLuint nextFrame() = 0;
};
#endif //TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
