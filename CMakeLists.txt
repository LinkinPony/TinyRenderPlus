cmake_minimum_required(VERSION 3.24)
project(TinyRenderPlus CXX)
set(CMAKE_CXX_STANDARD 23)
#add_compile_options(-fmodules-ts)

## Find dependencies
find_package(glfw3 REQUIRED)

add_subdirectory(UnitTest)

aux_source_directory(Render RENDER_SRC)
aux_source_directory(GUI GUI_SRC)
aux_source_directory(ThirdPartyLib/imgui/ IMGUI_SRC)
include_directories(
        .
        Render/
        GUI/
        ThirdPartyLib/
        ThirdPartyLib/stb/
        ThirdPartyLib/imgui
)
add_executable(TinyRenderPlus
        ${RENDER_SRC}
        ${GUI_SRC}
        ${IMGUI_SRC}
        main.cpp
)

target_link_libraries(TinyRenderPlus glfw3 GL dl X11 pthread)
#set_target_properties(TinyRenderPlus PROPERTIES LINKER_LANGUAGE CXX)
