//
// Created by linkinpony on 23-1-25.
//
//refer: https://stackoverflow.com/questions/15073259/converting-from-vector3f-to-matrix4f
#include "transform.h"
#include <iostream>

const float PI = acos(-1);
//remember, when you create a new matrix in Eigen, it's coefficients won't be initialized by default.
Eigen::Matrix4f Transform::viewTrans(const Eigen::Vector3f &e, const Eigen::Vector3f &c, const Eigen::Vector3f &t) {
  /*
   * e: Camera position point
   * c: Look-at center point
   * t: Up direction
   */
  Eigen::Vector3f g = (c - e);
  Eigen::Vector3f w = g.normalized();
  Eigen::Vector3f u = (t.cross(w)).normalized();
  Eigen::Vector3f v = w.cross(u);
  Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
  for (int i = 0; i < 3; i++) {
    T(i, 3) = -e[i];
  }
  Eigen::Matrix4f S;
  //TODO: change this
  S << u.x(), u.y(), u.z(), 0,
  v.x(), v.y(), v.z(), 0,
  -w.x(), -w.y(), -w.z(), 0,
  0, 0, 0, 1;
  //TODO: change this.
  return S * T;
//  return Eigen::Matrix4f::Identity();
}
Eigen::Matrix4f Transform::orthoTrans(float l, float r, float b, float t, float f, float n) {
  Eigen::Matrix4f m_orthoL,m_orthoR,m_ortho;
  m_orthoL << 2/(r - l), 0, 0, 0,
      0, 2/(t - b), 0, 0,
      0, 0, 2/(n - f), 0,
      0, 0, 0, 1;
  m_orthoR << 1, 0, 0, -(l+r)/2,
      0, 1, 0, -(b+t)/2,
      0, 0, 1, -(f+n)/2,
      0, 0, 0, 1;
  m_ortho = m_orthoL * m_orthoR;
  return m_ortho;
}
Eigen::Matrix4f Transform::perspTrans(float f, float n) {
  Eigen::Matrix4f m_persp2ortho;
  m_persp2ortho << n, 0, 0, 0,
      0, n, 0, 0,
      0, 0, n+f, -n*f,
      0, 0, 1, 0;
  return m_persp2ortho;
}
Eigen::Matrix4f Transform::viewportTrans(int width, int height) {
  Eigen::Matrix4f m_viewport;
  float w = width;
  float h = height;
  m_viewport <<
  w / 2, 0, 0, (w - 1) / 2,
  0, h / 2, 0, (h - 1) / 2,
  0, 0, 1, 0,
  0, 0, 0, 1;
  return m_viewport;
}
Eigen::Matrix4f Transform::modelTrans(float angle,float scale){
  Eigen::Matrix4f m_rotation;
  angle = angle * PI / 180.f;
  m_rotation << cos(angle), 0, sin(angle), 0,
      0, 1, 0, 0,
      -sin(angle), 0, cos(angle), 0,
      0, 0, 0, 1;

  Eigen::Matrix4f m_scale;
  m_scale <<
      scale, 0, 0, 0,
      0, scale, 0, 0,
      0, 0, scale, 0,
      0, 0, 0, 1;
  Eigen::Matrix4f m_translate = Eigen::Matrix4f::Identity();

  return m_translate * m_rotation * scale;
}
Eigen::Matrix4f Transform::projectionTrans(float eye_fov, float aspect_ratio, float zNear, float zFar){
  Eigen::Matrix4f m_projection = Eigen::Matrix4f::Identity();
  // Create the projection matrix for the given parameters.
  // Then return it.
  float halfV = eye_fov * PI / 180 / 2;
  float t = -tan(halfV) * zNear;//add negative sign to avoid upside-down problem
  float b = -t;
  float r = aspect_ratio * t;
  float l = -r;
  float n = zNear,f = zFar;
  auto m_persp2ortho = Transform::perspTrans(f,n);
  auto m_ortho = Transform::orthoTrans(l,r,b,t,f,n);
  m_projection = m_ortho * m_persp2ortho;
  return m_projection;
}
Eigen::Matrix4f Transform::getMVPMatrix
    (const Eigen::Vector3f &camera_position,
     const Eigen::Vector3f &view_center,
     float eyefov,
     float aspect_ratio,
     float zNear,
     float zFar,
     int image_width,
     int image_height) {
  //TODO: this is just a temp func
  //camera
  Eigen::Vector3f t(0, 1, 0);//Up direction
  Eigen::Matrix4f m_model = modelTrans(0,1);
  Eigen::Matrix4f m_view = viewTrans(camera_position, view_center, t);
  Eigen::Matrix4f m_projecton = projectionTrans(eyefov,aspect_ratio,zNear,zFar);
  Eigen::Matrix4f m_viewport = viewportTrans(image_width, image_height);
  Eigen::Matrix4f mvp = m_viewport * m_projecton * m_view * m_model;
  //TODO: delete debug output
  Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");
  std::cout << "zNear = " <<  zNear << std::endl;
  std::cout << mvp.format(CleanFmt) << "\n------------------\n";
  return mvp;
}
