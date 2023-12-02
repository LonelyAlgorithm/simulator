cmake_minimum_required(VERSION 3.12)
project(simulator)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(FreeGLUT REQUIRED HINTS "/opt/homebrew/Cellar/freeglut/3.4.0/include/GL")
include_directories(${FreeGLUT_INCLUDE_DIR})
find_package(geometry REQUIRED)
find_package(unit REQUIRED)

# include
include_directories(include)

add_subdirectory(src)
add_subdirectory(test)