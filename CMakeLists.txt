cmake_minimum_required(VERSION 3.0.0)
project(practice2 VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_executable(practice2 practice2.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
