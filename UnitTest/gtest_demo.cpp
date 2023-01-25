//
// Created by linkinpony on 23-1-7.
//
#include <gtest/gtest.h>
#include <object.h>
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
  std::cout << "Load object test.\n";
  std::unique_ptr<Object> obj = std::make_unique<Object>();
  obj->readObjFromFile("/home/linkinpony/Github/TinyRenderPlus/cmake-build-debug/models/diablo/diablo3_pose.obj");
  for(auto tri: obj->triangles_){
    for(int i = 0;i < 3;i++){
      for(int j = 0;j < 3;j++){
        std::cout << tri.v_[i][j] << " \n"[j == 2];
      }
    }
    std::cout << " ----- \n";
  }
  std::cout << "loaded " << obj->triangles_.size() <<  "triangles.\n";
}