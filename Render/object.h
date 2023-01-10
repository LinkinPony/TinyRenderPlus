 //
// Created by linkinpony on 23-1-9.
//

#ifndef TINYRENDERPLUS_RENDER_OBJECT_H_
#define TINYRENDERPLUS_RENDER_OBJECT_H_
#include <vector>
#include <string>
#include <eigen3/Eigen/Eigen>
class Object {
 private:
  std::vector<Eigen::Vector3f> verts_;//vertexs

  void readObjFromFile(std::string filename);
 public:

};

#endif //TINYRENDERPLUS_RENDER_OBJECT_H_
