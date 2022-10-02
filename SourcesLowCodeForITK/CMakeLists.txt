set(includes 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MockInclude.h
)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MockSource.cpp
)

add_library(${PROJECT_NAME} ${sources} ${includes}) #includes only to show in IDE

target_link_libraries(${PROJECT_NAME} PRIVATE QuickQanava)