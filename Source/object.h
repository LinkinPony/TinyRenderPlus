 //
// Created by linkinpony on 23-1-9.
//

#ifndef TINYRENDERPLUS_RENDER_OBJECT_H_
#define TINYRENDERPLUS_RENDER_OBJECT_H_
#include <vector>
#include <string>
#include <eigen3/Eigen/Eigen>
#include <triangle.h>
#include <tgaimage.h>
class Object {
 public:
  int id_;//id for texture
  std::string name_;
  Eigen::Vector4f position_;//Object's position in world coods
  Eigen::Vector3f direction_;//Object's direction vector. Need normalize.
  std::vector<Triangle> triangles_;//vertexs
  TGAImage diffuse_map_;
  TGAImage normal_map_;
  TGAImage specular_map_;
 public:
  int readObjFromFile(const std::string& filename);
};

#endif //TINYRENDERPLUS_RENDER_OBJECT_H_
