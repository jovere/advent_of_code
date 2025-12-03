cmake_minimum_required(VERSION 3.30)
project(Day3)

set(CMAKE_CXX_STANDARD 23)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
add_executable(Day3 main.cpp)
target_link_libraries(Day3 PRIVATE fmt)
