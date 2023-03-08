//
// Created by linkinpony on 23-1-8.
//

#include <editor.h>
#include <transform.h>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#endif
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ThirdPartyLib/imgui/imgui.h"
#include "ThirdPartyLib/imgui/imgui_impl_glfw.h"
#include "ThirdPartyLib/imgui/imgui_impl_opengl3.h"

static std::shared_ptr<Editor> editor;

void glmouseCallbackWrapper(GLFWwindow* window, double xpos, double ypos) {
  if (editor == nullptr) return;
  editor->mouseCallback(xpos, ypos);
}

void glscrollCallbackWrapper(GLFWwindow* window, double xoffset,
    double yoffset) {
  if (editor == nullptr) return;
  editor->scrollCallback(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  return;
}
void Editor::loadScene(std::shared_ptr<Scene> scene) {
  // TODO: move this to somewhere else
  this->scene_ = scene;
  config_ = scene_->get_config();
  config_->camera_->getref_camera_direction() = Eigen::Vector3f(0, 0, -1);
  config_->camera_->getref_camera_position() = Eigen::Vector3f(0, 0, 3);
  config_->camera_->getref_up_direction() = Eigen::Vector3f(0, 1, 0);
  config_->light_position = Eigen::Vector3f(200, 200, 200);
  config_->light_intensity = 500;
  config_->fov = 45;
  config_->zNear = 0.1;
  config_->zFar = 100;
  auto light1 = Light(config_->light_position, config_->light_intensity);
  scene->addLight(light1);
}
void processInput(GLFWwindow* window, std::shared_ptr<Camera> camera);
void bindVector3fToInputBox(Eigen::Vector3f& target,
                            const std::string& target_name, float v_min,
                            float v_max) {
  ImGui::SliderFloat((target_name + ".x").c_str(), &target.x(), v_min, v_max);
  ImGui::SliderFloat((target_name + ".y").c_str(), &target.y(), v_min, v_max);
  ImGui::SliderFloat((target_name + ".z").c_str(), &target.z(), v_min, v_max);
}

int Editor::run() {
  editor = this->shared_from_this();
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(screen_width_, screen_height_,
                                        "TinyRender Plus", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create window." << std::endl;
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

  float last_time = glfwGetTime();
  std::shared_ptr<Camera> camera = scene_->get_config()->camera_;


  while (!glfwWindowShouldClose(window)) {
    float cur_time = glfwGetTime();
    float delta_time = cur_time - last_time;
    camera->set_move_speed(delta_time * 0.01);
    camera->moveByEulerianAngles(config_->pitch,config_->yaw);
    std::cout << config_->yaw << " " << config_->pitch << std::endl;
    if (capture_cursor_flag_ == true) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(window, glmouseCallbackWrapper);
      glfwSetScrollCallback(window, glscrollCallbackWrapper);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(window, nullptr);
      glfwSetScrollCallback(window, nullptr);
    }
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
    processInput(window,scene_->get_config()->camera_);
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



void Editor::buildConfigWidget() {
  ImGui::Begin("Config");
  ImGui::Text((std::string("Capture cursor: ") +
               (editor->capture_cursor_flag_ ? "YES" : "NO")).c_str());
  //ImGui::Text(config_->getInfoString().c_str());
  ImGui::Text(scene_->getMatrixInfo().c_str());
  bindVector3fToInputBox(
      scene_->get_config()->camera_->getref_camera_direction(),
      "camera direction", -50, 50);
  bindVector3fToInputBox(
      scene_->get_config()->camera_->getref_camera_position(),
      "camera position", -50, 50);
  bindVector3fToInputBox(scene_->get_config()->camera_->getref_up_direction(),
                         "up direction", -1, 1);
  
  ImGui::End();
}

void Editor::mouseCallback(double xpos, double ypos) {
  static float last_x = screen_width_ / 2;
  static float last_y = screen_height_ / 2;
  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;
  float sensitivity = 0.01;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  auto& yaw = config_->yaw;
  auto& pitch = config_->pitch;
  yaw += xoffset;
  pitch += yoffset;
  // TODO: use const var
  if (pitch > 89.0f) {
    pitch = 89;
  }
  if (pitch < -89.0f) {
    pitch = -89;
  }
}

void Editor::scrollCallback(double xoffset, double yoffset) {
  float& fov = config_->fov;
  if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
  if (fov <= 1.0f) fov = 1.0f;
  if (fov >= 45.0f) fov = 45.0f;
}

void processInput(GLFWwindow* window, std::shared_ptr<Camera> camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kForward);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kLeft);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kBack);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kRight);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kUp);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera->moveByDirection(Camera::MoveDirection::kDown);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
    editor->capture_cursor_flag_ = !editor->capture_cursor_flag_;
  }
}