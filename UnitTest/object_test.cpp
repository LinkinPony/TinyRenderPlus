//
// Created by linkinpony on 23-1-23.
//
#include <gtest/gtest.h>
#include <object.h>

TEST(ObjectTest, LoadObject){
  //load diablo_pose.obj, have 5022 triangles.
  //TODO: change this stupid abs path
  std::unique_ptr<Object> obj = std::make_unique<Object>();
  obj->readObjFromFile("/home/linkinpony/Github/TinyRenderPlus/cmake-build-debug/models/diablo/diablo3_pose.obj");
  EXPECT_EQ(obj->triangles_.size(),5022);
}