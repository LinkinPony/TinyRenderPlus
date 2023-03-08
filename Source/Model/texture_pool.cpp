#include "texture_pool.h"
#include <iostream>
std::shared_ptr<TGAImage> TexturePool::loadTextureByPath(const std::string& path) {
  auto it = pointer_map_.find(path);
  if (it != pointer_map_.end()) {
    return it->second;
  } 
  // TODO: avoid copy
  std::shared_ptr<TGAImage> temp = std::make_unique<TGAImage>(TGAImage());
  bool ok = temp->read_tga_file(path.c_str());
  std::cerr << "texture file " << path << " loading "
            << (ok ? "ok" : "failed") << std::endl;
  std::cerr << "size[h x w]: " << temp->get_height() << "x" << temp->get_width()
            << std::endl;
  temp->flip_vertically();
  if (!ok) {
    std::cout << path << " "
              << "load failed." << std::endl;
    TGAColor white = TGAColor(255, 255, 255, 255);
    temp->operator=(white);
  }
  return pointer_map_[path] = temp;
}

