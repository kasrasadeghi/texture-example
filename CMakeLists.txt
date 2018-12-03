cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# When in debug mode, we run Address Sanitizer to find memory errors
set(CMAKE_CXX_FLAGS "-fmax-errors=1 -Wno-unused-parameter")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(cmake/glad_glfw.cmake)
include(cmake/utils.cmake)
include(cmake/freetype.cmake)

set(PROJECT_NAME fortress-commander)
project(${PROJECT_NAME})

set(SRC_FILES
  glad/src/glad.c
  src/Graphics/ResourceManager.cpp
  src/Graphics/Shader.cpp
  src/Graphics.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})

setup_glad_glfw(${PROJECT_NAME})
setup_freetype(${PROJECT_NAME})

# copy over resources
copy_to_bin_dir(shaders textures)
