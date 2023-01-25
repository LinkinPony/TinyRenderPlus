cmake_minimum_required(VERSION 3.24)
project(TinyRenderPlus CXX)
set(CMAKE_CXX_STANDARD 23)
#add_compile_options(-fmodules-ts)

## Find dependencies
find_package(glfw3 REQUIRED)

add_subdirectory(UnitTest)

aux_source_directory(Source SOURCE_SRC)
aux_source_directory(GUI GUI_SRC)
aux_source_directory(ThirdPartyLib/imgui/ IMGUI_SRC)
aux_source_directory(ThirdPartyLib/tinyrender TINY_RENDER_SRC)
include_directories(
        .
        Source
        Source/Shader
        Source/Render
        GUI
        ThirdPartyLib
        ThirdPartyLib/stb
        ThirdPartyLib/imgui
        ThirdPartyLib/tinyrender
)
add_executable(TinyRenderPlus
        ${SOURCE_SRC}
        ${GUI_SRC}
        ${IMGUI_SRC}
        ${TINY_RENDER_SRC}
        main.cpp
        Source/Render/render.h Source/Shader/shader.h Source/Shader/shader_data.h Source/Shader/shader_pipeline.cpp Source/Shader/shader_pipeline.h Source/Shader/bling_phong_shader.cpp Source/Shader/bling_phong_shader.h Source/Shader/shader.cpp Source/Shader/transform.cpp Source/Shader/transform.h)

target_link_libraries(TinyRenderPlus glfw3 GL dl X11 pthread)
#set_target_properties(TinyRenderPlus PROPERTIES LINKER_LANGUAGE CXX)
