# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.12.4)

project (TomatoEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}  "Main.cpp"  "ThirdParty/VulkanMemoryAllocator/include/vk_mem_alloc.h" "ThirdParty/VulkanMemoryAllocator/src/Common.cpp" "ThirdParty/VulkanMemoryAllocator/src/Common.h" "ThirdParty/VulkanMemoryAllocator/src/SparseBindingTest.cpp" "ThirdParty/VulkanMemoryAllocator/src/SparseBindingTest.h" "ThirdParty/VulkanMemoryAllocator/src/Tests.cpp" "ThirdParty/VulkanMemoryAllocator/src/Tests.h" "ThirdParty/VulkanMemoryAllocator/src/VmaUsage.cpp" "ThirdParty/VulkanMemoryAllocator/src/VmaUsage.h" "ThirdParty/VulkanMemoryAllocator/src/VulkanSample.cpp")

set(ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(GLFW_DIR ${ENGINE_DIR}/ThirdParty/glfw)
set(GLAD_DIR ${ENGINE_DIR}/ThirdParty/glad)
set(IMGUI_DIR ${ENGINE_DIR}/ThirdParty/imgui)
set(GLM_DIR ${ENGINE_DIR}/ThirdParty/glm)
set(STB_DIR ${ENGINE_DIR}/ThirdParty/stb)
set(ENTT_DIR ${ENGINE_DIR}/ThirdParty/entt)
set(JSON_DIR ${ENGINE_DIR}/ThirdParty/json)
set(IMGUIZMO_DIR ${ENGINE_DIR}/ThirdParty/ImGuizmo)
set(ASSIMP_BIN_DEBUG_DIR ${ENGINE_DIR}/ThirdParty/assimp/bin/Debug)
set(ASSIMP_BIN_RELEASE_DIR ${ENGINE_DIR}/ThirdParty/assimp/bin/Release)
set(ASSIMP_INCLUDE_DIR ${ENGINE_DIR}/ThirdParty/assimp/include)

add_definitions(-DTMT_PLATFORM_WINDOWS)

add_subdirectory(Tomato)
add_subdirectory(Editor)
add_subdirectory(ThirdParty)
add_subdirectory(PreCompile)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} 
	Tomato
	Editor
)




