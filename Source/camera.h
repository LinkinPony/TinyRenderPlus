#pragma once
#ifndef TINYRENDERPLUS_RENDER_CAMERA_H_
#define TINYRENDERPLUS_RENDER_CAMERA_H_

#include "transform.h"

class Camera {
 private:
  Eigen::Vector3f camera_position_;
  Eigen::Vector3f camera_target_;
  Eigen::Vector3f camera_direction_;
  Eigen::Vector3f up_direction_;
  Eigen::Vector3f camera_right_direction_;
  Eigen::Vector3f camera_up_direction_;

  float move_speed_ = 0.05;

 private:
  void updateCameraStatus();

 public:
  enum class MoveDirection { kLeft, kRight, kForward, kBack, kUp, kDown };

 public:
  explicit Camera(const Eigen::Vector3f& camera_position,
                  const Eigen::Vector3f& camera_direction,
                  const Eigen::Vector3f& up_direction);
  Camera();
  Eigen::Matrix4f getLookAtMat();
  void moveByDirection(MoveDirection direction);
  void moveByEulerianAngles(float pitch_degree, float yaw_degree);

 public:
  void set_camera_position(const Eigen::Vector3f& camera_position) {
    camera_position_ = camera_position;
    updateCameraStatus();
  }
  void set_camera_target(const Eigen::Vector3f& camera_target) {
    camera_target_ = camera_target;
    updateCameraStatus();
  }
  void set_up_direction(const Eigen::Vector3f& up_direction) {
    up_direction_ = up_direction;
    updateCameraStatus();
  }
  void set_camera_direction(const Eigen::Vector3f& camera_direction) {
    camera_direction_ = camera_direction;
    updateCameraStatus();
  }
  void set_move_speed(float speed) { move_speed_ = speed; }

 public:
  Eigen::Vector3f& getref_camera_position() { return camera_position_; }
  Eigen::Vector3f& getref_camera_target() { return camera_target_; }
  Eigen::Vector3f& getref_camera_direction() { return camera_direction_; }
  Eigen::Vector3f& getref_up_direction() { return up_direction_; }
  Eigen::Vector3f& getref_camera_right_direction() {
    return camera_right_direction_;
  }
  Eigen::Vector3f& get_camera_up_direction() { return camera_up_direction_; }
  float get_move_speed() { return move_speed_; }
};

#endif TINYRENDERPLUS_RENDER_CAMERA_H_