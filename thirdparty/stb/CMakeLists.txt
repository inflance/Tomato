cmake_minimum_required(VERSION 3.8)

project(stb)

add_library(stb "stb_image.h" "stb_image.cpp")

target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})