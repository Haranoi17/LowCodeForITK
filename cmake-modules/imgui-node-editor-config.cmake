set(IMGUI_NODE_EDITOR_BUILD_DIR ${PROJECT_SOURCE_DIR}/buildExternals/Build-imgui-node-editor)
set(IMGUi_NODE_EDITOR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/externals/imgui-node-editor)

set(imgui-node-editor_LIBRARY_DIRS ${IMGUI_NODE_EDITOR_BUILD_DIR}/canvas-example/${CMAKE_BUILD_TYPE})
set(imgui-node-editor_LIBRARIES 
${imgui-node-editor_LIBRARY_DIRS}/imgui_node_editor.lib
${IMGUI_NODE_EDITOR_BUILD_DIR}/application/${CMAKE_BUILD_TYPE}/application.lib
d3d11
)
set(imgui-node-editor_RESOURCE_DIR ${IMGUI_NODE_EDITOR_BUILD_DIR}/bin/data)
set(imgui-node-editor_INCLUDE_DIRS
    ${IMGUi_NODE_EDITOR_SOURCE_DIR}
    ${IMGUi_NODE_EDITOR_SOURCE_DIR}/examples/application/include
    ${IMGUI_NODE_EDITOR_BUILD_DIR}/application/source
    )