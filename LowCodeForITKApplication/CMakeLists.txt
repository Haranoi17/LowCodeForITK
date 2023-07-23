set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE sources *.cpp )
file(GLOB_RECURSE headers *.hpp *.h)

set(TargetName ${PROJECT_NAME}Application)

add_executable(${TargetName} ${sources} ${headers})

target_link_libraries(${TargetName} PRIVATE 
nlohmann_json::nlohmann_json
imgui_node_editor
application
range-v3
${ITK_LIBRARIES}
)

target_include_directories(${TargetName} PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../externals/imgui-filebrowser/

)

if(MSVC)
target_link_options(${TargetName} PRIVATE /ENTRY:WinMainCRTStartup)
endif()