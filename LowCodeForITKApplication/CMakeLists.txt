set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.hpp *.h)

set(TargetName ${PROJECT_NAME}Application)

add_executable(${TargetName} ${sources} ${headers})

target_link_libraries(${TargetName} PRIVATE 
    ${IMGUI_NODE_EDITOR_LIBRARIES}
    nlohmann_json::nlohmann_json
    range-v3::range-v3
    ${ITK_LIBRARIES}
    d3d11.lib
)

target_include_directories(${TargetName} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    ${IMGUI_NODE_EDITOR_INCLUDE_DIRS}
    ${IMGUI_INCLUDE_DIRS}
    ${filebrowser_INCLUDE_DIRS}
)

if(MSVC)
target_link_options(${TargetName} PRIVATE /ENTRY:WinMainCRTStartup)
endif()