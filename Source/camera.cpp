#include "camera.h"

const float PI = acos(-1.0);

inline float radians(float degree) { return degree * PI / 180.0f; }

void Camera::updateCameraStatus() {
  // camera_direction_ = glm::normalize(camera_direction_);
  camera_target_ = camera_position_ + camera_direction_;
  camera_right_direction_ =
      (camera_direction_.cross(world_up_direction_)).normalized();
  camera_up_direction_ =
      (camera_right_direction_.cross(camera_direction_)).normalized();
}

Camera::Camera(const Eigen::Vector3f& camera_position,
               const Eigen::Vector3f& camera_direction,
               const Eigen::Vector3f& up_direction)
    : camera_position_(camera_position),
      camera_direction_(camera_direction.normalized()),
      up_direction_(up_direction),
      world_up_direction_(up_direction) {
  updateCameraStatus();
}

Camera::Camera() {
  camera_position_ = Eigen::Vector3f(0, 0, 3);
  camera_direction_ = Eigen::Vector3f(0, 0, -1);
  up_direction_ = Eigen::Vector3f(0, 1, 0);
  world_up_direction_ = Eigen::Vector3f(0, 1, 0);
  updateCameraStatus();
}

Eigen::Matrix4f Camera::getLookAtMat() {
  return Transform::viewTrans(getref_camera_position(), getref_camera_target(),
                              getref_up_direction());
}

void Camera::moveByDirection(MoveDirection direction) {
  Eigen::Vector3f cam_pos = getref_camera_position();
  float speed = get_move_speed();
  switch (direction) {
    case Camera::MoveDirection::kLeft:
      cam_pos -= speed * getref_camera_right_direction();
      break;
    case Camera::MoveDirection::kRight:
      cam_pos += speed * getref_camera_right_direction();
      break;
    case Camera::MoveDirection::kForward:
      cam_pos += speed * getref_camera_direction();
      break;
    case Camera::MoveDirection::kBack:
      cam_pos -= speed * getref_camera_direction();
      break;
    case Camera::MoveDirection::kUp:
      cam_pos += speed * getref_up_direction();
      break;
    case Camera::MoveDirection::kDown:
      cam_pos -= speed * getref_up_direction();
      break;
    default:
      break;
  }
  set_camera_position(cam_pos);
}

void Camera::moveByEulerianAngles(float pitch_degree, float yaw_degree) {
  if (pitch_degree > 89.0f) {
    pitch_degree = 89.0f;
  }
  if (pitch_degree < -89.0f) {
    pitch_degree = -89.0f;
  }
  Eigen::Vector3f direction;
  float pitch_radian = radians(pitch_degree);
  float yaw_radian = radians(yaw_degree);
  direction.x() = cos(pitch_radian) * cos(yaw_radian);
  direction.y() = sin(pitch_radian);
  direction.z() = cos(pitch_radian) * sin(yaw_radian);
  direction = direction.normalized();
  set_camera_direction(direction);
  updateCameraStatus();
}
