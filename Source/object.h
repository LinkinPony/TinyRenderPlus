 //
// Created by linkinpony on 23-1-9.
//

#ifndef TINYRENDERPLUS_RENDER_OBJECT_H_
#define TINYRENDERPLUS_RENDER_OBJECT_H_
#include <vector>
#include <string>
#include <memory>
#include <eigen3/Eigen/Eigen>
#include <triangle.h>
#include <tgaimage.h>
class Object {
 public:
  int id_;//id for texture
  std::string name_;
  Eigen::Vector4f position_;//Object's position in world coods
  Eigen::Vector3f direction_;//Object's direction vector. Need normalize.
  std::vector<Triangle> triangles_;//
  //TODO: change TGAImage to point.
  std::shared_ptr<TGAImage> diffuse_map_;
  std::shared_ptr<TGAImage> normal_map_;
  std::shared_ptr<TGAImage> specular_map_;
 public:
  int readObjFromFile(const std::string &filename);
  int readDiffusemap(const std::string &filename);
  int readNormalmap(const std::string &filename);
  int readSpecularmap(const std::string &filename);
  Object(){}
  Object(const std::string & filename){
    //TODO: check if file exists
    readObjFromFile(filename + ".obj");
    readDiffusemap(filename);
    readNormalmap(filename);
  }
 private:
  std::unique_ptr<TGAImage> readTGA(const std::string &filename);
  const std::string FILE_NORMAL_MAP_SUFFIX_ = "_nm.tga";
  const std::string FILE_DIFFUSE_SUFFIX_ = "_diffuse.tga";
};

#endif //TINYRENDERPLUS_RENDER_OBJECT_H_
