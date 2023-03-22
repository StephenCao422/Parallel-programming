cmake_minimum_required(VERSION 3.19)
project(Multiple_thread)
set(CMAKE_CXX_STANDARD 14)

add_executable(Multiple_thread Sobel_Edge_Detection.cpp)
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

target_link_libraries(Multiple_thread ${OpenCV_LIBS})

