cmake_minimum_required(VERSION 3.30)
project(Day2)

set(CMAKE_CXX_STANDARD 23)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
add_executable(Day2 main.cpp)
target_link_libraries(Day2 PRIVATE fmt)
