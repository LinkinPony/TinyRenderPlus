#include "model.h"


std::vector<Texture> Model::loadMaterialTexture(aiMaterial* material,
                                                aiTextureType ai_type,
                                                Texture::texture_type type) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(ai_type); i++) {
    aiString ai_path;
    material->GetTexture(ai_type, i, &ai_path);
    std::string path(ai_path.C_Str());
    auto texture_ptr = loadTextureFromFile(path.c_str(), directory_);
    textures.emplace_back(texture_ptr, path, type);
  }
  return textures;
}

std::shared_ptr<TGAImage> Model::loadTextureFromFile(
    const std::string& path, const std::string& directory) {
  if (texture_pool_ == nullptr) {
    std::cerr << "[ERROR] : texture pool failed to load." << std::endl;
    return nullptr;
  }
  // TODO: use custom file system
  return texture_pool_->loadTextureByPath(directory + "/" + path);
}

Model::Model(const std::string& filepath,
             std::shared_ptr<TexturePool> texture_pool,
             const Eigen::Matrix4f& m_model)
    : texture_pool_(texture_pool), m_model_(m_model) {
  loadModel(filepath);
}

Model::Model(const std::vector<Mesh>& mesh, const Eigen::Matrix4f& m_model)
    : mesh_(mesh), m_model_(m_model) {
  texture_pool_ = nullptr;
}

void Model::draw(std::shared_ptr<Render> render) {
  for (auto& it : mesh_) {
    it.draw(render);
  }
}

void Model::set_m_model(const Eigen::Matrix4f& m_model) {
  m_model_ = m_model;
  for (auto& it : mesh_) {
    it.set_m_model(m_model_);
  }
}

void Model::loadModel(const std::string& path) {
  Assimp::Importer importer;
  auto scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !(scene->mRootNode)) {
    std::cout << "[error] assimp: " << importer.GetErrorString() << std::endl;
  } else {
    std::cout << "assimp loaded model" << path << std::endl;
  }
  // TODO : change this symbol '/'
  directory_ = path.substr(0, path.find_last_of('/'));
  if (scene) {
    processNode(scene->mRootNode, scene);
  }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    auto mesh_ptr = scene->mMeshes[node->mMeshes[i]];
    mesh_.push_back(processMesh(mesh_ptr, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    auto node_ptr = node->mChildren[i];
    processNode(node_ptr, scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  // process vertex
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Eigen::Vector3f pos(mesh->mVertices[i].x, mesh->mVertices[i].y,
                        mesh->mVertices[i].z);
    // TODO: change this
    assert(mesh->HasNormals());
    Eigen::Vector3f norm(mesh->mNormals[i].x, mesh->mNormals[i].y,
                         mesh->mNormals[i].z);
    auto tex_coord = Eigen::Vector2f(0.0f, 0.0f);
    if (mesh->mTextureCoords[0]) {
      tex_coord = Eigen::Vector2f(mesh->mTextureCoords[0][i].x,
                                  mesh->mTextureCoords[0][i].y);
    }
    vertices.emplace_back(Vertex(pos, norm, tex_coord));
  }
  // process index
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    auto& face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
  // process texture
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuse_maps =
        loadMaterialTexture(material, aiTextureType_DIFFUSE, Texture::kDiffuse);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    auto specular_maps = loadMaterialTexture(material, aiTextureType_SPECULAR,
                                             Texture::kSpecular);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }
  return Mesh(vertices, indices, textures, m_model_);
}
