cmake_minimum_required(VERSION 3.20)
project(finalproject)

add_executable(sum src/sum.cpp)
target_compile_features(sum PRIVATE cxx_std_17)

add_executable(latency src/latency.cpp)
target_compile_features(latency PRIVATE cxx_std_17)

add_executable(bandwidth src/bandwidth.cpp)
target_compile_features(bandwidth PRIVATE cxx_std_17)

