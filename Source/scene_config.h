#ifndef TINYRENDERPLUS_RENDER_SCENE_CONFIG_H_
#define TINYRENDERPLUS_RENDER_SCENE_CONFIG_H_
class SceneConfig {
 public:
  // IMPORTANT: set default value
  // Scene
  int width = 0;
  int height = 0;
  float scale = 0;
  float fov = 0;           // vertical fov
  float aspect_ratio = 0;  // aspect_ration = width / height
  float zNear = 10;
  float zFar = 300;
  float yaw = 0;
  float pitch = 0;
  std::shared_ptr<Camera> camera_;
  SceneConfig(int width, int height, float fov)
      : width(width), height(height), fov(fov) {
    aspect_ratio = static_cast<float>(width) / height;
  }
  std::string getInfoString();
  // Camera
  // Light
  // TODO: maybe you can move lights and other TGAImage to resource pack.
  // TODO: use multi lights.
  Eigen::Vector3f light_position = Eigen::Vector3f::Zero();
  float light_intensity = 0;

  [[maybe_unused]] void debugPrint();
};
#endif TINYRENDERPLUS_RENDER_SCENE_CONFIG_H_