//
// Created by linkinpony on 23-1-25.
//
// refer:
// https://stackoverflow.com/questions/15073259/converting-from-vector3f-to-matrix4f
#include "transform.h"

#include <iostream>

const float PI = acos(-1);
// remember, when you create a new matrix in Eigen, it's coefficients won't be
// initialized by default.
Eigen::Matrix4f Transform::viewTrans(const Eigen::Vector3f &camera_position,
                                     const Eigen::Vector3f &camera_target,
                                     const Eigen::Vector3f &up_direction) {
  /*
   * e: Camera position point
   * c: Look-at center point
   * t: Up direction
   */
  auto e = camera_position;
  auto g = (camera_target - camera_position);
  auto t = up_direction.normalized();

  Eigen::Vector3f w = g.normalized();
  Eigen::Vector3f u = (t.cross(w)).normalized();
  Eigen::Vector3f v = (w.cross(u)).normalized();
  Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
  for (int i = 0; i < 3; i++) {
    T(i, 3) = -e[i];
  }
  Eigen::Matrix4f S;
  S << u.x(), u.y(), u.z(), 0, v.x(), v.y(), v.z(), 0, -w.x(), -w.y(), -w.z(),
      0, 0, 0, 0, 1;
  return S * T;
}
Eigen::Matrix4f Transform::orthoTrans(float l, float r, float b, float t,
                                      float f, float n) {
  Eigen::Matrix4f m_orthoL, m_orthoR, m_ortho;

  m_orthoL << 2 / (r - l), 0, 0, 0, 0, 2 / (t - b), 0, 0, 0, 0, 2 / (n - f), 0,
      0, 0, 0, 1;
  m_orthoR << 1, 0, 0, -(l + r) / 2, 0, 1, 0, -(b + t) / 2, 0, 0, 1,
      -(f + n) / 2, 0, 0, 0, 1;
  m_ortho = m_orthoL * m_orthoR;
  return m_ortho;
}
Eigen::Matrix4f Transform::perspTrans(float f, float n) {
  Eigen::Matrix4f m_persp2ortho;
  m_persp2ortho << n, 0, 0, 0, 0, n, 0, 0, 0, 0, n + f, -n * f, 0, 0, 1, 0;
  return m_persp2ortho;
}
Eigen::Matrix4f Transform::viewportTrans(int width, int height) {
  Eigen::Matrix4f m_viewport;
  float w = width;
  float h = height;
  m_viewport << w / 2, 0, 0, (w - 1) / 2, 0, h / 2, 0, (h - 1) / 2, 0, 0, 1, 0,
      0, 0, 0, 1;
  return m_viewport;
}
Eigen::Matrix4f Transform::modelTrans(float angle, float scale) {
  Eigen::Matrix4f m_rotation;
  angle = angle * PI / 180.f;
  m_rotation << cos(angle), 0, sin(angle), 0, 0, 1, 0, 0, -sin(angle), 0,
      cos(angle), 0, 0, 0, 0, 1;

  Eigen::Matrix4f m_scale;
  m_scale << scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1;
  Eigen::Matrix4f m_translate = Eigen::Matrix4f::Identity();

  return m_translate * m_rotation * scale;
}
Eigen::Matrix4f Transform::projectionTrans(float eye_fov, float aspect_ratio,
                                           float zNear, float zFar) {
  Eigen::Matrix4f m_projection = Eigen::Matrix4f::Identity();
  // Create the projection matrix for the given parameters.
  // Then return it.
  float halfV = eye_fov * PI / 180 / 2;
  float t = tan(halfV) * zNear;  // add negative sign to avoid upside-down
  float r = aspect_ratio * t;
  float n = zNear;
  float f = zFar;
  m_projection(0, 0) = n / r;
  m_projection(1, 1) = n / t;
  m_projection(2, 2) = (n + f) / (n - f);
  m_projection(2, 3) = (2 * n * f) / (n - f);
  m_projection(3, 2) = 1;
  return m_projection;
}
