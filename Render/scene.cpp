//
// Created by linkinpony on 23-1-8.
//

#include <scene.h>
void Scene::nextFrame() {
  //TODO: lots of work to do
  memset(image_data_.get(),0xff,sizeof(char) * width_ * height_ * 3);

}
int Scene::get_height() {
  return height_;
}
int Scene::get_width() {
  return width_;
}
void Scene::set_height(int height) {
  height_ = height;
  aspect_ratio_ = static_cast<float>(width_) / height_;
}
void Scene::set_width(int width) {
  width_ = width;
  aspect_ratio_ = static_cast<float>(width_) / height_;
}
void Scene::genTextureInit() {
  glGenTextures(1, &render_result_);
  glBindTexture(GL_TEXTURE_2D, render_result_);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

}
bool Scene::loadTextureFromMemory(){

  // Load from file
//  raw_image_width = ShaderGlobal::current_drawer->get_width();
//  raw_image_height = ShaderGlobal::current_drawer->get_height();
//  image_data = ShaderGlobal::current_drawer -> data;
  if (image_data_ == nullptr)
    return false;
  // Create a OpenGL texture identifier
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image_data_.get());
  return true;
}

GLuint Scene::get_render_result(){
  return render_result_;
}