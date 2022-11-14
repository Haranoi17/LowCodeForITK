set(IMGUI_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/externals/imgui-node-editor)
set(imgui_LIBRARY_DIRS ${IMGUI_EXTERNAL_DIR}/build/external/imgui/${CMAKE_BUILD_TYPE})
set(imgui_LIBRARIES 
    ${imgui_LIBRARY_DIRS}/imgui.lib
    )
set(imgui_INCLUDE_DIRS
    ${IMGUI_EXTERNAL_DIR}/external/imgui
)