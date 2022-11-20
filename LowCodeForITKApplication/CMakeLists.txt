set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE sources *.cpp *.hpp *.h)

set(TargetName ${PROJECT_NAME}Application)

add_executable(${TargetName} ${sources})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

target_link_libraries(${TargetName} PRIVATE 
imgui_node_editor::imgui_node_editor 
imgui_node_editor::imgui 
imgui_node_editor::application
)

target_include_directories(${TargetName} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Identification
    ${CMAKE_INSTALL_PREFIX}/../imgui_node_editor/include/imgui_node_editor
    ${CMAKE_INSTALL_PREFIX}/../imgui_node_editor/include/imgui
    ${CMAKE_INSTALL_PREFIX}/../imgui_node_editor/include/application
    )

target_link_options(${TargetName} PRIVATE /ENTRY:WinMainCRTStartup)