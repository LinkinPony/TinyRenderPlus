#ifndef TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
#define TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
//interface of render. could be rasterizer or ray tracing or something interesting.
#include <memory>
#include <GLES2/gl2.h>
#include <tgaimage.h>
#include <mesh.h>
#include <scene.h>
#include <shader.h>
#include <scene.h>
#include <scene_config.h>
class Render{
 public:
  virtual void newFrame() = 0;
  virtual std::shared_ptr<TGAImage> getRenderResult() = 0;
  virtual void drawMesh(const Mesh& mesh) = 0;
  virtual void applySceneConfig(std::shared_ptr<SceneConfig> config,
                                std::shared_ptr<Shader> shader) = 0;
};
#endif //TINYRENDERPLUS_SOURCE_RENDER_RENDER_H_
