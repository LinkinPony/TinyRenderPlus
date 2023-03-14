cmake_minimum_required(VERSION 3.22)
project(TinyRenderPlus CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -O2)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3")
#set(CMAKE_CXX_FLAGS  "-pg")
#set(CMAKE_CXX_FLAGS  "-fopenmp")
#add_compile_options(-fmodules-ts)

# Find dependencies
find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
#find_package(unofficial-angle REQUIRED CONFIG)
find_package(benchmark REQUIRED CONFIG)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

#add_subdirectory(UnitTest)

aux_source_directory(Source SOURCE_SRC)
aux_source_directory(Source/Model SOURCE_MODEL_SRC)
aux_source_directory(Source/Render SOURCE_RENDER_SRC)
aux_source_directory(Source/Shader SOURCE_SHADER_SRC)
aux_source_directory(GUI GUI_SRC)
aux_source_directory(ThirdPartyLib/imgui/ IMGUI_SRC)
aux_source_directory(ThirdPartyLib/tinyrender TINY_RENDER_SRC)
include_directories(
        .
        Source
        Source/Shader
        Source/Render
        Source/Model
        GUI
        ThirdPartyLib
        ThirdPartyLib/stb
        ThirdPartyLib/imgui
        ThirdPartyLib/tinyrender
)
add_executable(TinyRenderPlus
        ${SOURCE_SRC}
        ${SOURCE_MODEL_SRC}
        ${SOURCE_SHADER_SRC}
        ${SOURCE_RENDER_SRC}
        ${GUI_SRC}
        ${IMGUI_SRC}
        ${TINY_RENDER_SRC}
        main.cpp
        Source/Render/render.h Source/Shader/shader.h Source/Shader/shader_data.h Source/Shader/bling_phong_shader.cpp Source/Shader/bling_phong_shader.h Source/Shader/shader.cpp Source/Shader/transform.cpp Source/Shader/transform.h Source/camera.cpp Source/camera.h "Source/geometry.cpp" "Source/Model/model.h" "Source/Model/model.cpp" "Source/Model/mesh.h" "Source/Model/mesh.cpp" "Source/Render/rasterizer.h" "Source/Render/rasterizer.cpp" "Source/scene_config.h" "Source/Model/texture_pool.h" "Source/Model/texture_pool.cpp" Source/Render/thread_pool.cpp)

target_link_libraries(TinyRenderPlus glad::glad glfw imgui::imgui Eigen3::Eigen benchmark::benchmark assimp::assimp)
#set_target_properties(TinyRenderPlus PROPERTIES LINKER_LANGUAGE CXX)

