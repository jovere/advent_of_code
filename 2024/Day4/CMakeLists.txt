cmake_minimum_required(VERSION 3.30)
project(Day4)

set(CMAKE_CXX_STANDARD 20)

find_package( Boost REQUIRED)
add_executable(Day4 main.cpp)
#target_link_libraries(Day4 PRIVATE Boost::regex)