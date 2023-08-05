cmake_minimum_required(VERSION 3.24)
project( LowCodeForITK )

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/LowCodeForITKApplication)

include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/range-v3)

set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor/examples)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LowCodeForITKApplication)
