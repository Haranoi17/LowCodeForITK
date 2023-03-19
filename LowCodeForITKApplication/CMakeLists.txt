set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.hpp *.h)

set(TargetName ${PROJECT_NAME}Application)

add_executable(${TargetName} ${sources} ${headers})

target_link_libraries(${TargetName} PRIVATE 
    imgui_node_editor::imgui_node_editor 
    imgui_node_editor::imgui 
    imgui_node_editor::application
    imgui_node_editor::stb_image
    imgui-filebrowser::imgui-filebrowser
    ${ITK_LIBRARIES}
)

target_include_directories(${TargetName} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_INSTALL_PREFIX}/../imgui_node_editor/include/"
    "${CMAKE_INSTALL_PREFIX}/../imgui-filebrowser/include/"
    "${CMAKE_INSTALL_PREFIX}/../ITK/include/"
    "${CMAKE_INSTALL_PREFIX}/../nlohmann_json/include"
    "${CMAKE_INSTALL_PREFIX}/../Range-v3/include"
)

if(MSVC)
target_link_options(${TargetName} PRIVATE /ENTRY:WinMainCRTStartup)
endif()