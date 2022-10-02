set(mock_test_source_files mockTest.cpp )
set(mock_test_qrc_source_files qml.qrc)

set(mock_test_sources ${mock_test_source_files} ${mock_test_qrc_source_files})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)


add_executable(mockTest ${mock_test_sources})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(mockTest PRIVATE ${QuickQanava_INCLUDE_DIRS})

# target_link_directories(mockTest PUBLIC ${QuickQanava_LIBRARY_DIRS})
target_link_libraries(mockTest PUBLIC QuickQanava Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
set_target_properties(mockTest PROPERTIES
    WIN32_EXECUTABLE TRUE
)
# target_include_directories(mockTest PUBLIC Qt${QT_VERSION_MAJOR}::QuickControls2)
# target_link_libraries(mockTest Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::QuickControls2)

