set(_imgui_node_editor_SourceDir ${IMGUI_NODE_EDITOR_ROOT_DIR}/examples)
set(_imgui_node_editor_BinaryDir ${CMAKE_BINARY_DIR}/external/imgui_node_editor)

set(IMGUI_NODE_EDITOR_INCLUDE_DIRS 
 
${IMGUI_NODE_EDITOR_ROOT_DIR}
${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/application/include 
${IMGUI_NODE_EDITOR_ROOT_DIR}/external/imgui
)
set(_DXSDK_Dir ${IMGUI_NODE_EDITOR_ROOT_DIR}/external/DXSDK)

set(_DXSDK_Arch x86)
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(_DXSDK_Arch x64)
endif()

add_library(dxerr STATIC ${_DXSDK_Dir}/src/dxerr.cpp)
target_include_directories(dxerr PUBLIC "${_DXSDK_Dir}/include")
set_property(TARGET dxerr PROPERTY FOLDER "external")

add_library(d3dx11 UNKNOWN IMPORTED)
set_target_properties(d3dx11 PROPERTIES
    IMPORTED_LOCATION               "${_DXSDK_Dir}/lib/${_DXSDK_Arch}/d3dx11.lib"
    IMPORTED_LOCATION_DEBUG         "${_DXSDK_Dir}/lib/${_DXSDK_Arch}/d3dx11d.lib"
    INTERFACE_INCLUDE_DIRECTORIES   "${_DXSDK_Dir}/include"
    INTERFACE_LINK_LIBRARIES        "$<$<CONFIG:Debug>:dxerr>"
)

set(IMGUI_NODE_EDITOR_LIBRARIES 
${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/build/canvas-example/Debug/imgui_node_editor.lib 
${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/build/external/imgui/Debug/imgui.lib
${IMGUI_NODE_EDITOR_ROOT_DIR}/examples/build/application/Debug/application.lib
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    imgui_node_editor
    REQUIRED_VARS
        IMGUI_NODE_EDITOR_INCLUDE_DIRS
        IMGUI_NODE_EDITOR_LIBRARIES
)

