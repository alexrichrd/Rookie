cmake_minimum_required(VERSION 3.20)
project(Chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic -g)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(Chess ${SRC_FILES})

set_property(TARGET Chess PROPERTY CXX_STANDARD 23)
