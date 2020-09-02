cmake_minimum_required(VERSION 3.16.5)
project(LearnOpenGL)

# set standard with 11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address -fno-omit-frame-pointer")

# include files
include_directories(/opt/X11/include)
include_directories(./deps/glad/include/)
include_directories(./deps/stb_image/)
include_directories(./deps/glm/)
include_directories(./utils/)
include_directories(/usr/local/Cellar/glfw/3.2.1/include/)

aux_source_directory(./deps/glad/src/ glad_src)
aux_source_directory(./utils/ utils_src)

link_directories(/usr/local/Cellar/glfw/3.2.1/lib/)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lglfw")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglfw")

# compile test file
file(GLOB_RECURSE src_cpps ./src/*.cpp)
foreach(src_exe ${src_cpps})
  get_filename_component(mainname ${src_exe} NAME_WE)
  add_executable(${mainname} ${src_exe} ${glad_src} ${utils_src})
endforeach()
