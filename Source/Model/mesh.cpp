#include "mesh.h"
#include "render.h"
const static std::map<Texture::texture_type, std::string> type2name = {
    {Texture::texture_type::kDiffuse, "diffuse"},
    {Texture::texture_type::kSpecular, "specular"}};
Mesh::Mesh(const std::vector<Vertex> vertices,
           const std::vector<unsigned int> indices,
           const std::vector<Texture> textures)
    : vertices_(vertices), indices_(indices), textures_(textures) {
}

void Mesh::draw(std::shared_ptr<Render> render) { render->drawMesh(*this); }