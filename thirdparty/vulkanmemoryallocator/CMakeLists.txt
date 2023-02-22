cmake_minimum_required(VERSION 3.8)

project(VulkanMemoryAllocator)

add_library(VulkanMemoryAllocator "vk_mem_alloc.h" "vk_mem_alloc.cpp")

target_include_directories(VulkanMemoryAllocator 
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} 
PUBLIC C:/VulkanSDK/1.3.231.1/include)

target_link_libraries(VulkanMemoryAllocator
	 C:/VulkanSDK/1.3.231.1/Lib/vulkan-1.lib
)