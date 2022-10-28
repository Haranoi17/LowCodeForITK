cmake_minimum_required(VERSION 3.24)
project( LowCodeForITK )

#QT-------------------------------------------------------------------------
#these variables have to be set before finding QT to make qml "qrc:/" paths work
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml Quick QuickControls2 REQUIRED)
#---------------------------------------------------------------------------

#QuickQanava----------------------------------------------------------------
#You have to install QuickQanava first in your system
#this gives me only QuickQanava_DIR
find_package(QuickQanava REQUIRED)

#so defining inclues myself
set(QuickQanava_INCLUDE_DIRS 
    ${QuickQanava_DIR}/include/quickqanava
    ${QuickQanava_DIR}/include/quickcontainers/include
    ${QuickQanava_DIR}/include/quickcontainers)

set(QuickQanava_LIBRARY_DIRS ${QuickQanava_DIR}/lib)
#---------------------------------------------------------------------------

#ITK------------------------------------------------------------------------
#You have to install ITK first in your system
find_package(ITK REQUIRED)
#---------------------------------------------------------------------------

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourcesLowCodeForITK)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)


include(${CMAKE_CURRENT_SOURCE_DIR}/utils.cmake) #for debuging purpose
# print_all_variables() # from utils.cmake
