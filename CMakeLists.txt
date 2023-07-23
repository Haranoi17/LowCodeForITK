cmake_minimum_required(VERSION 3.24)
project( LowCodeForITK )

# get_directory_property(_vars VARIABLES)

# list(REMOVE_ITEM _vars _vars_before ${_vars_before})
# foreach(_var IN LISTS _vars)
#     message(STATUS "${_var} = ${${_var}}")
# endforeach()

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/LowCodeForITKApplication)

include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/range-v3)

set(IMGUI_NODE_EDITOR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/imgui-node-editor/examples)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LowCodeForITKApplication)
# print_all_variables() # from utils.cmake
