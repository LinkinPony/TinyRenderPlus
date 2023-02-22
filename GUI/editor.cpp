//
// Created by linkinpony on 23-1-8.
//

#include <editor.h>
#include <transform.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ThirdPartyLib/imgui/imgui.h"
#include "ThirdPartyLib/imgui/imgui_impl_glfw.h"
#include "ThirdPartyLib/imgui/imgui_impl_opengl3.h"

const int screen_width = 1280;
const int screen_height = 720;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  return;
}
void Editor::loadScene(std::shared_ptr<Scene> scene) {
  // TODO: move this to somewhere else
  this->scene_ = scene;
  auto config = scene_->get_config();
  config->camera_position = Eigen::Vector3f(0, 0, 3);
  config->camera_direction = Eigen::Vector3f(0, 0, -1);
  config->up_direction = Eigen::Vector3f(0, 1, 0);
  config->light_position = Eigen::Vector3f(200, 200, 200);
  config->light_intensity = 500;
  config->fov = 45;
  config->zNear = 0.1;
  config->zFar = 100;
  auto light1 = Light(config->light_position, config->light_intensity);
  scene->addLight(light1);
}

int Editor::run() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(screen_width, screen_height,
                                        "Demo window", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Create window failed." << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // render result
  glGenTextures(1, &render_result);
  scene_->nextFrame();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, render_result);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene_->get_width(),
               scene_->get_height(), 0, GL_BGR, GL_UNSIGNED_BYTE,
               scene_->get_render_buffer()->data);

  // Imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    scene_->nextFrame();  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render_result);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene_->get_width(),
                 scene_->get_height(), 0, GL_BGR, GL_UNSIGNED_BYTE,
                 scene_->get_render_buffer()->data);
    //
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Show render result
    buildRenderResultWidget();
    buildConfigWidget();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return 0;
}

void Editor::buildRenderResultWidget() {
  ImGui::Begin("Render result");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Image((void*)(intptr_t)render_result,
               ImVec2(scene_->get_width(), scene_->get_height()));
  ImGui::End();
}

void bindVectorToInputBox(Eigen::Vector3f & target,const std::string & target_name) {

    ImGui::
}

void Editor::buildConfigWidget() {
  ImGui::Begin("Config");
  auto config = scene_->get_config();



  
  ImGui::InputFloat3()


  ImGui::End();
}
