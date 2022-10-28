set(includes 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MockInclude.h
)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MockSource.cpp
)

add_library(${PROJECT_NAME} ${sources} ${includes})

target_link_libraries(${PROJECT_NAME} PRIVATE QuickQanava)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)