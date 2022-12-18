cmake_minimum_required(VERSION 3.22)
project( LowCodeForITK )

#ITK------------------------------------------------------------------------
#You have to install ITK first in your system
find_package(ITK REQUIRED)
include( ${ITK_USE_FILE} )
#---------------------------------------------------------------------------

#ImGuiNodeEditor------------------------------------------------------------\
# set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor)
# set(imgui-node-editor_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
# set(imgui_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
# find_package(imgui REQUIRED)
# find_package(imgui-node-editor REQUIRED)
#---------------------------------------------------------------------------

find_package(imgui_node_editor REQUIRED)
find_package(imgui-filebrowser REQUIRED)
#gtest----------------------------------------------------------------------
# find_package(gtest REQUIRED)
#---------------------------------------------------------------------------

include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LowCodeForITKApplication)
# print_all_variables() # from utils.cmake
