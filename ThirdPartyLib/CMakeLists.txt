#cmake_minimum_required(VERSION 3.24)
#project(ThirdPartyLib CXX)
#set(CMAKE_CXX_STANDARD 23)
#aux_source_directory(ThirdPartyLib/imgui/
#        IMGUI_SRC)
#add_library (ThirdPartyLib
#        tiny_obj_loader.h
#        stb/stb_image.h
#        ${IMGUI_SRC}
#        )
#set_target_properties(ThirdPartyLib PROPERTIES LINKER_LANGUAGE CXX)