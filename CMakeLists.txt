cmake_minimum_required(VERSION 3.10)
project(snapper)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
find_library(FlyCap_LIBS flycapture)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(
    snapper
    snapper.cpp
    show.hpp
    snap.hpp
    time.hpp
    )
target_link_libraries(snapper ${OpenCV_LIBS} ${FlyCap_LIBS})
