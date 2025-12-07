cmake_minimum_required(VERSION 3.30)
project(Day7)

set(CMAKE_CXX_STANDARD 23)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
add_executable(Day7 main.cpp)
target_link_libraries(Day7 PRIVATE fmt)
