#cmake_minimum_required(VERSION 3.24)
project(UnitTest)

#---add origin project---
aux_source_directory(../Source SOURCE_SRC)
aux_source_directory(../GUI GUI_SRC)
aux_source_directory(../ThirdPartyLib/imgui/ IMGUI_SRC)
aux_source_directory(../ThirdPartyLib/tinyrender TINY_RENDER_SRC)
#include_directories(
#        ../
#        ../Source
#        ../Source/Shader
#        ../Source/Render
#        ../GUI
#        ../ThirdPartyLib
#        ../ThirdPartyLib/stb
#        ../ThirdPartyLib/imgui
#        ../ThirdPartyLib/tinyrender
#)
#---finish adding---

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

aux_source_directory(. CURRENT_SRC)

add_executable(
        UnitTest
        ${CURRENT_SRC}
        ${SOURCE_SRC}
        ${GUI_SRC}
        ${IMGUI_SRC}
        ${TINY_RENDER_SRC}
        gtest_demo.cpp
        )
target_link_libraries(
        UnitTest
        GTest::gtest_main
)

include(GoogleTest)
#gtest_discover_tests(gtest_demo)