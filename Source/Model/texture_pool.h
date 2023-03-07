#pragma once
#include <tgaimage.h>

#include <map>
#include <memory>
#include <vector>
#include <set>
// TODO: write test case for thi s

// maintain points of TGAImages(texture)
// support load, delete and get
class TexturePool {
 public:
  // check if Texture has been loaded, if not, load it and
  // return raw_id. otherwise just return raw_id
  size_t loadTextureByPath(const std::string& path);
  // delete this id and free texture resource
  void deleteTextureByID(size_t raw_id);
  // get texture pointer by id
  std::shared_ptr<TGAImage> getTextureByID(size_t raw_id);

 private:
  // indexed by id.
  std::vector<std::shared_ptr<TGAImage>> texture_ptr_;
  // When one texture is deleted, their id is
  // freed and push into there.
  // It's promised each element in free_id_ is less than texture_ptr_.size()
  std::set<size_t> free_id_;
  // In shrink(), there will be some element in free_id_ which is greater than
  // or equal to new size. we need move all of them into delete_free_id.
  std::set<size_t> delete_free_id_;
  // texture is identified by their path.
  std::map<std::string, size_t> path2id_;

  // About raw_id and id:
  // We return raw_id to texture pool user. This id will never change until it
  // was deleted.
  // However, to reduce memory consume, we use id to indicate where the pointer
  // really is. So id may change by shrink() operation. We use rawid2id to
  // maintain a map of <raw_id,id>.
  std::vector<size_t> rawid2id_;

  float expand_factor_ = 2;
  float shrink_factor = 0.25;

 private:
  // when (free space) / (total space) is smaller than shrink_factor_, we shrink
  // texture_ptr_'s size to size * shrink_factor * 2 just like std::vector do.
  // We also need to remap rawid2id_.
  void shrink();
  // when there's no free space (free_id_.size() == 0), we need to expand
  // texture_ptr's size to size * expand_factor. We also need to insert free
  // indecies into free_id_. 
  void expand();

 private:
};