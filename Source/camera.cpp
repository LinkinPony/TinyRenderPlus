#include "camera.h"

const float PI = acos(-1.0);

inline float radians(float degree) { return degree * PI / 2.0f; }

void Camera::updateCameraStatus() {
  // camera_direction_ = glm::normalize(camera_direction_);
  camera_target_ = camera_position_ + camera_direction_;
  camera_right_direction_ =
      (up_direction_.cross(camera_direction_)).normalized();
  camera_up_direction_ =
      (camera_direction_.cross(camera_right_direction_)).normalized();
}

Camera::Camera(const Eigen::Vector3f& camera_position,
               const Eigen::Vector3f& camera_direction,
               const Eigen::Vector3f& up_direction)
    : camera_position_(camera_position),
      camera_direction_(camera_direction.normalized()),
      up_direction_(up_direction.normalized()) {
  updateCameraStatus();
}

Camera::Camera() {
  auto camera_position = Eigen::Vector3f(0, 0, -10);
  auto camera_direction = Eigen::Vector3f(0, 0, 1);
  auto up_direction = Eigen::Vector3f(0, 1, 0);
  Camera(camera_position, camera_direction, up_direction);
}

Eigen::Matrix4f Camera::getLookAtMat() {
  return Transform::viewTrans(get_camera_position(), get_camera_target(),
                              get_up_direction());
}

void Camera::moveByDirection(MoveDirection direction) {
  auto cam_pos = get_camera_position();
  float speed = get_move_speed();
  switch (direction) {
    case Camera::MoveDirection::kLeft:
      cam_pos -= speed * get_camera_right_direction();
      break;
    case Camera::MoveDirection::kRight:
      cam_pos += speed * get_camera_right_direction();
      break;
    case Camera::MoveDirection::kForward:
      cam_pos += speed * get_camera_direction();
      break;
    case Camera::MoveDirection::kBack:
      cam_pos -= speed * get_camera_direction();
      break;
    case Camera::MoveDirection::kUp:
      cam_pos += speed * get_up_direction();
      break;
    case Camera::MoveDirection::kDown:
      cam_pos -= speed * get_up_direction();
      break;
    default:
      break;
  }
  set_camera_position(cam_pos);
}

void Camera::moveByEulerianAngles(float pitch_degree, float yaw_degree) {
  Eigen::Vector3f direction;
  float pitch_radian = radians(pitch_degree);
  float yaw_radian = radians(yaw_degree);
  direction.x() = -cos(pitch_radian) * cos(yaw_radian);
  direction.y() = -sin(pitch_radian);
  direction.z() = cos(pitch_radian) * sin(yaw_radian);
  direction = direction.normalized();
  set_camera_direction(direction);
}
