cmake_minimum_required(VERSION 3.24)
project( LowCodeForITK )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(ITK REQUIRED)
include( ${ITK_USE_FILE} )

find_package(Range-v3 REQUIRED)
find_package(nlohmann_json REQUIRED)

set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor)
find_package(imgui_node_editor MODULE REQUIRED)
find_package(imgui-filebrowser MODULE REQUIRED)


# get_directory_property(_vars VARIABLES)

# list(REMOVE_ITEM _vars _vars_before ${_vars_before})
# foreach(_var IN LISTS _vars)
#     message(STATUS "${_var} = ${${_var}}")
# endforeach()

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/LowCodeForITKApplication)

include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LowCodeForITKApplication)
# print_all_variables() # from utils.cmake
