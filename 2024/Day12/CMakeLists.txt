cmake_minimum_required(VERSION 3.30)
project(Day12)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
add_executable(Day12 main.cpp)
target_link_libraries(Day12 PRIVATE fmt)