#ifndef TINYRENDERPLUS_RENDER_MESH_H_
#define TINYRENDERPLUS_RENDER_MESH_H_
#include <Shader/shader.h>

#include <eigen3/Eigen/Eigen>
#include <map>
#include <memory>
#include <string>
#include <vector>
class Render;
struct Vertex {
  Eigen::Vector3f position;
  Eigen::Vector3f normal;
  Eigen::Vector2f tex_coord;
  Vertex(const Eigen::Vector3f& position_in, const Eigen::Vector3f& normal_in,
         const Eigen::Vector2f& tex_coord_in)
      : position(position_in), normal(normal_in), tex_coord(tex_coord_in) {}
  void debugPrint() const {
    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
    std::cout << position.format(CleanFmt) << std::endl;
  }
};
struct Texture {
  enum texture_type { kDiffuse, kSpecular };
  // use type2name to get texture's name in shader.
  // kDiffuse = "diffuse", kSpecular = "specular"
  // name rule in shader: type T's i-th texture is
  // texture_type2name[T]_i
  // for example, 3th(numbered from zero) specular texture's name is
  // texture_specular_2
  const static std::map<texture_type, std::string> type2name;
  std::shared_ptr<TGAImage> image;
  std::string path;
  texture_type type;
  Texture(std::shared_ptr<TGAImage> image_in, const std::string& path_in,
          texture_type type_in)
      : image(image_in), path(path_in), type(type_in) {}
};
class Mesh {
 public:
  void draw(std::shared_ptr<Render> render);
  Mesh(const std::vector<Vertex> vertices,
       const std::vector<unsigned int> indices,
       const std::vector<Texture> textures, const Eigen::Matrix4f& m_model);
  ~Mesh() = default;

 public:
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const std::vector<unsigned int>& get_indices() const { return indices_; }
  const std::vector<Texture>& get_textures() const { return textures_; }
  const Eigen::Matrix4f& get_m_model() const { return m_model_; }
  void set_m_model(const Eigen::Matrix4f& m_model) { m_model_ = m_model; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  Eigen::Matrix4f m_model_;
};
#endif  // TINYRENDERPLUS_RENDER_MESH_H_