cmake_minimum_required(VERSION 3.0.0)
project(Project1 VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")
add_executable(Project1 project1.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
