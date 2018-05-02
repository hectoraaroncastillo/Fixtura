cmake_minimum_required(VERSION 2.8)
project( main )
find_package( OpenCV REQUIRED )
file(GLOB SOURCES
    include/preProcess.hpp
    src/preProcess.cpp
)
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBRARIES OFF)
##SET(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(main main.cpp src/preProcess.cpp)
target_link_libraries( main ${OpenCV_LIBS} )
