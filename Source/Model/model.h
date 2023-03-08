#ifndef TINYRENDERPLUS_RENDER_MODEL_H_
#define TINYRENDERPLUS_RENDER_MODEL_H_

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <mesh.h>
#include <texture_pool.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string>
#include <vector>

class Model {
 public:
  Model(const std::string& filepath, std::shared_ptr<TexturePool> texture_pool,
        const Eigen::Matrix4f& m_model);
  Model(const std::vector<Mesh>& mesh, const Eigen::Matrix4f& m_model);
  void draw(std::shared_ptr<Render> render);
  void set_m_model(const Eigen::Matrix4f& m_model);

 private:
  std::vector<Mesh> mesh_;
  std::string directory_;
  Eigen::Matrix4f m_model_;
  std::shared_ptr<TexturePool> texture_pool_;

 private:
  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTexture(aiMaterial* material,
                                           aiTextureType ai_type,
                                           Texture::texture_type type);
  std::shared_ptr<TGAImage> loadTextureFromFile(const std::string& path,
                                                const std::string& directory);
};
#endif  // TINYRENDERPLUS_RENDER_MODEL_H_