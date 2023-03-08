#include "texture_pool.h"


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <ThirdPartyLib/stb/stb_image.h>
#endif

#include <iostream>
std::shared_ptr<TGAImage> TexturePool::loadTextureByPath(const std::string& path) {
  //TODO: use stb_image to read this.
}

