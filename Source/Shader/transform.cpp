//
// Created by linkinpony on 23-1-25.
//
//refer: https://stackoverflow.com/questions/15073259/converting-from-vector3f-to-matrix4f
#include "transform.h"
//remember, when you create a new matrix in Eigen, it's coefficients won't be initialized by default.
Eigen::Matrix4f Transform::viewTrans(const Eigen::Vector3f &e, const Eigen::Vector3f &c, const Eigen::Vector3f &t) {
  /*
   * e: Camera position point
   * c: Look-at center point
   * t: Up direction
   */
  Eigen::Vector3f g = (c - e).normalized();
  Eigen::Vector3f w = Eigen::Vector3f(-g.x(), -g.y(), -g.z());
  Eigen::Vector3f u = (t.cross(w)).normalized();
  Eigen::Vector3f v = w.cross(u);
  Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
  for (int i = 0; i < 3; i++) {
    T(i, 3) = -e[i];
  }
  Eigen::Matrix4f S;
  //TODO: change this
  S.matrix().row(0) << u.x(), u.y(), u.z(), 0;
  S.matrix().row(1) << v.x(), v.y(), v.z(), 0;
  S.matrix().row(2) << -w.x(), -w.y(), -w.z(), 0;
  S.matrix().row(3) << 0, 0, 0, 1;
  return S * T;
}
Eigen::Matrix4f Transform::orthoTrans(float l, float r, float b, float t, float f, float n) {
  Eigen::Matrix4f S = Eigen::Matrix4f::Identity();
  Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
  S(0, 0) = 2 / (r - l);
  S(1, 1) = 2 / (t - b);
  S(2, 2) = 2 / (n - f);
  S(3, 3) = 1;
  T(0, 3) = -(l + r) / 2;
  T(1, 3) = -(b + t) / 2;
  T(2, 3) = -(f + n) / 2;
  return S * T;
}
Eigen::Matrix4f Transform::perspTrans(float l, float r, float b, float t, float f, float n) {
  Eigen::Matrix4f M_ortho = orthoTrans(l, r, b, t, f, n);
  Eigen::Matrix4f M_p2o = Eigen::Matrix4f::Zero();
  M_p2o(0, 0) = M_p2o(1, 1) = n;
  M_p2o(2, 2) = n + f;
  M_p2o(2, 3) = -n * f;
  M_p2o(3, 2) = 1;
  return M_ortho * M_p2o;
}
Eigen::Matrix4f Transform::viewportTrans(int width, int height) {
  Eigen::Matrix4f res = Eigen::Matrix4f::Zero();
  res(0, 0) = res(0, 0) = (float) width / 2.0f;
  res(1, 1) = res(1, 3) = (float) height / 2.0f;
  res(2, 2) = res(3, 3) = 1;
  return res;
}
Eigen::Matrix4f Transform::getMVPMatrix
    (const Eigen::Vector3f &camera_position,
     const Eigen::Vector3f &view_center,
     int image_width,
     int image_height) {
  //TODO: this is just a temp func
//  Eigen::Matrix4f M_model = identityMatrix4();
//  M_model(0,0) = M_model(1,1) = M_model(2,2) = 1000;//multiply .obj's coordinate by 500
//  M_model(0,3) = 1000,M_model(1,3) = 1000;
//  M_model(2,3) = -1000;
//  M_model(3,3) = 1;
  int near = -300, far = -10000;
//  //camera
  Eigen::Vector3f t(0, 1, 0);//Up direction
  Eigen::Matrix4f M_view = viewTrans(camera_position, view_center, t);
  Eigen::Matrix4f M_perse = perspTrans(0, image_width, 0, image_height, far, near);
  Eigen::Matrix4f M_viewport = viewportTrans(image_width, image_height);
  Eigen::Matrix4f M = M_viewport * M_view;
  return M;
}
