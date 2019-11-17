cmake_minimum_required(VERSION 3.15)
project(LambdaThreadPool)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads)
add_executable(LambdaThreadPool main.cpp)
target_link_libraries(LambdaThreadPool pthread)