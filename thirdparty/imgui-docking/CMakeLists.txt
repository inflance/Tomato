cmake_minimum_required(VERSION 3.8)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(imgui)

file(GLOB imgui_sources CONFIGURE_DEPENDS  "${IMGUI_DIR}/*.cpp" "${IMGUI_DIR}/*.h")
file(GLOB imgui_impl CONFIGURE_DEPENDS  
"${IMGUI_DIR}/backends/imgui_impl_glfw.cpp" 
"${IMGUI_DIR}/backends/imgui_impl_glfw.h"
"${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp" 
"${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp" 
"${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp" 
"${IMGUI_DIR}/backends/imgui_impl_vulkan.h")

add_library(imgui STATIC ${imgui_sources} ${imgui_impl})

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${Vulkan_DIR}/include)
target_link_libraries(imgui PUBLIC glfw opengl32 ${Vulkan_DIR}/Lib/vulkan-1.lib)




