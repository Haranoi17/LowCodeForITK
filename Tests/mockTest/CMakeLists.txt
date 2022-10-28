set(mock_test_source_files mockTest.cpp )
set(mock_test_qrc_source_files qml.qrc)

set(mock_test_sources ${mock_test_source_files} ${mock_test_qrc_source_files})

add_executable(mockTest ${mock_test_sources})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_include_directories(mockTest PRIVATE ${QuickQanava_INCLUDE_DIRS} ${ITK_INCLUDE_DIRS})

target_link_libraries(mockTest PRIVATE ${ITK_LIBRARIES} QuickQanava Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)