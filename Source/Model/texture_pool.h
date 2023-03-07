#include <tgaimage.h>

#include <map>
#include <memory>

class TexturePool {
  // maintain points of TGAImages(texture)
  // support load, delete and get

 public:
  // check if Texture has been loaded, if not, load it and
  // return id. otherwise just return id
  size_t loadTextureByPath(const std::string& path);
  //delete this id and free texture resource
  void deleteTextureByID(size_t id);
  //get texture pointer by id
  std::shared_ptr<TGAImage> getTextureByID(size_t id);
 private:
  vector<std::shared_ptr<TGAImage>> texture_ptr_;  // indexed by id
  vector<size_t> free_id_;  // when one texture is deleted, their id is freed
                            // and push into there
  std::map<string, size_t> path2id_;  // texture is identified by their path.
};