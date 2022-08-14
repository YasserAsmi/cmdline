cmake_minimum_required(VERSION 3.0.0)
project(wstex VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_executable(cmdtest main.cpp)
