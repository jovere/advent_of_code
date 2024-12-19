cmake_minimum_required(VERSION 3.30)
project(Day13)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(Eigen3 REQUIRED)
add_executable(Day13 main.cpp)
target_link_libraries(Day13 PRIVATE fmt Eigen3::Eigen)