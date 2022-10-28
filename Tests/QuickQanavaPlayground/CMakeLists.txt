set(PlaygroundTarget QuickQanavaPlayground )
add_executable(${PlaygroundTarget} main.cpp qml.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
target_include_directories(${PlaygroundTarget} PRIVATE ${QuickQanava_INCLUDE_DIRS})
target_link_libraries(${PlaygroundTarget} PRIVATE QuickQanava)
target_compile_features(${PlaygroundTarget} PRIVATE cxx_std_20)