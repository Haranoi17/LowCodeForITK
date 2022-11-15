set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE sources *.cpp *.hpp *.h)

set(TargetName ${PROJECT_NAME}Application)

add_executable(${TargetName} ${sources})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

set(INCLUDE_DIRS 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/Identification
)

target_include_directories(${TargetName} PUBLIC 
${INCLUDE_DIRS}
${imgui-node-editor_INCLUDE_DIRS}
${imgui_INCLUDE_DIRS}
)

target_link_libraries(${TargetName} PUBLIC ${imgui-node-editor_LIBRARIES} ${imgui_LIBRARIES})

target_link_options(${TargetName} PRIVATE /ENTRY:WinMainCRTStartup)

file(COPY ${imgui-node-editor_RESOURCE_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${imgui-node-editor_RESOURCE_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
