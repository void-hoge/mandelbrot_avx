cmake_minimum_required(VERSION 3.25)

project(mandelbrot)

set(CMAKE_CXX_STANDARD 20)
set("CMAKE_CXX_FLAGS" "-DNDEBUG -O3 -mtune=native -march=native")

find_package(OpenMP)
if(OpenMP_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(mandelbrot mandelbrot.cpp util.cpp main.cpp)
