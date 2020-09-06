cmake_minimum_required(VERSION 3.16.3)
project(LearnOpenGL)

# set standard with 11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# include files
include_directories(./deps/glad/include/)
include_directories(./deps/stb_image/)
include_directories(./deps/glm/)
include_directories(./src/utils/)

aux_source_directory(./deps/glad/src/ glad_src)
aux_source_directory(./src/utils/ utils_src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

link_libraries("-lglfw -lX11 -ldl")

# compile test file
file(GLOB_RECURSE src_cpps ./src/tutorials/*.cpp)
foreach(src_exe ${src_cpps})
  get_filename_component(mainname ${src_exe} NAME_WE)
  add_executable(${mainname} ${src_exe} ${glad_src} ${utils_src})
endforeach()
