set(PlaygroundTarget firstITKPlayground )
add_executable(${PlaygroundTarget} main.cpp qml.qrc)

target_include_directories(${PlaygroundTarget} PRIVATE ${ITK_INCLUDE_DIRS})
target_link_libraries(${PlaygroundTarget} PRIVATE ${ITK_LIBRARIES} )
target_compile_features(${PlaygroundTarget} PRIVATE cxx_std_20)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})