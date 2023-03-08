#pragma once
#include <tgaimage.h>

#include <map>
#include <memory>
#include <vector>
#include <set>
// TODO: write test case for thi s

// maintain points of TGAImages(texture)
class TexturePool {
 public:
  // check if Texture has been loaded, if not, load it and
  // return pointer. otherwise just return pointer
   std::shared_ptr<TGAImage> loadTextureByPath(const std::string& path);
  TexturePool() = default;
  ~TexturePool() = default;
   TexturePool(const TexturePool&) = delete;
  TexturePool(TexturePool&&) = delete;
 private:
  // indexed by id.
  std::map<std::string, std::shared_ptr<TGAImage>> pointer_map_;
};