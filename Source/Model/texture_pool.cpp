#include "texture_pool.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <ThirdPartyLib/stb/stb_image.h>
#endif

#include <iostream>
#include <string>
std::shared_ptr<TGAImage> TexturePool::loadTextureByPath(
    const std::string& path) {
  std::cout << "Loading texture " << path << std::endl;
  int width, height, nrChannels;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    std::cout << "Texture width x height = " << width << " x " << height
              << std::endl;
    TGAImage::Format format;
    switch (nrChannels) {
      case 1:
        format = TGAImage::GRAYSCALE;
        break;
      case 3:
        format = TGAImage::RGB;
        break;
      case 4:
        format = TGAImage::RGBA;
        break;
      default:
        format = TGAImage::RGBA;
        break;
    }
    auto ptr = std::make_shared<TGAImage>(width, height, nrChannels);
    memcpy(ptr->data, data, width * height * nrChannels);
    //ptr->flip_vertically();
    return ptr;
  } else {
    std::cout << "Loading failed" << std::endl;
  }
  return std::make_shared<TGAImage>();
}
