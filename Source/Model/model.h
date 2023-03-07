#ifndef TINYRENDERPLUS_RENDER_MODEL_H_
#define TINYRENDERPLUS_RENDER_MODEL_H_

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string>
#include <vector>

#include "mesh.h"

class Model {
 public:
  Model(const std::string& filepath);
  Model(const std::vector<Mesh>& mesh);
  void draw(std::shared_ptr<Render> render);

 public:
  void set_m_model(const Eigen::Matrix4f& mat) { m_model_ = mat;}

 private:
  std::vector<Mesh> mesh_;
  std::string directory_;
  Eigen::Matrix4f m_model_;

 private:
  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTexture(aiMaterial* material,
                                           aiTextureType ai_type,
                                           Texture::texture_type type);
 int loadTextureFromFile(const std::string& path,
                             const std::string& directory);
};
#endif //TINYRENDERPLUS_RENDER_MODEL_H_