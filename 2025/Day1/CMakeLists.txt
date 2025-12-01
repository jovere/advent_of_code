cmake_minimum_required(VERSION 3.30)
project(Day1)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
add_executable(Day1 main.cpp)
target_link_libraries(Day1 PRIVATE fmt)
