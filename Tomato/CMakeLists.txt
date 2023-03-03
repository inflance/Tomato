cmake_minimum_required (VERSION 3.12.4)

project("Tomato")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DTMT_PLATFORM_WINDOWS)

file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

file(GLOB_RECURSE ALL_HEAD "*.hpp" "*.h")

add_library(Tomato  ${ALL_SOURCE} ${ALL_HEAD})

target_precompile_headers(
	${PROJECT_NAME}
	PUBLIC pch.h
)

file(GLOB imguiInitFile "./ImGui/imgui.ini")
file(COPY ${imguiInitFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}/
	PUBLIC ${ENTT_DIR}/single_include
	PUBLIC ${ENTT_DIR}/src
	PUBLIC ${JSON_DIR}/single_include
	PUBLIC ${IMGUIZMO_DIR}
	PUBLIC ${ASSIMP_INCLUDE_DIR}
	PUBLIC ${VMA_DIR}
	PUBLIC ${Vulkan_DIR}/include
	PUBLIC ${TINY_OBJ_LOAD_DIR}
	PUBLIC ${BOX2D_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC glfw
	PUBLIC spdlog
	PUBLIC glad
	PUBLIC ${IMGUI_DIR}
	PUBLIC glm
	PUBLIC stb
	PUBLIC EnTT
	PUBLIC nlohmann_json
	PUBLIC ImGuizmo
	PUBLIC VulkanMemoryAllocator
	PUBLIC utils
	PUBLIC RAII_utils
	PUBLIC box2d
	PUBLIC ${TINY_OBJ_LOAD_DIR}
	PUBLIC ${ASSIMP_BIN_DEBUG_DIR}/assimp-vc142-mtd.lib
	PUBLIC ${ASSIMP_BIN_RELEASE_DIR}/assimp-vc142-mt.lib
	${Vulkan_DIR}/Lib/vulkan-1.lib
	${Vulkan_DIR}/Lib/glslang.lib
	${Vulkan_DIR}/Lib/shaderc_util.lib
	${Vulkan_DIR}/Lib/shaderc_shared.lib
	${Vulkan_DIR}/Lib/shaderc.lib
	opengl32
)



