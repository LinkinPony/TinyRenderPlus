cmake_minimum_required(VERSION 3.10)
project(TinyRenderPlus CXX)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(ThirdPartyLib)
add_subdirectory(UnitTest)

add_executable(TinyRenderPlus
        main.cc
        )
target_link_libraries(ThirdPartyLib)