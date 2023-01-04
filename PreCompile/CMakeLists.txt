cmake_minimum_required (VERSION 3.8)

project (PreCompile)

file(GLOB allCopyFiles  "./*")

file(COPY ${allCopyFiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


