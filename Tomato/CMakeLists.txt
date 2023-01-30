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
	PUBLIC C:/VulkanSDK/1.3.231.1/include
)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC glfw
	PUBLIC spdlog
	PUBLIC glad
	PUBLIC imgui
	PUBLIC glm
	PUBLIC stb
	PUBLIC EnTT
	PUBLIC nlohmann_json
	PUBLIC ImGuizmo
	PUBLIC ${ASSIMP_BIN_DEBUG_DIR}/assimp-vc142-mtd.lib
	PUBLIC ${ASSIMP_BIN_RELEASE_DIR}/assimp-vc142-mt.lib
	 C:/VulkanSDK/1.3.231.1/Lib/vulkan-1.lib
	 C:/VulkanSDK/1.3.231.1/Lib/glslang.lib
	 C:/VulkanSDK/1.3.231.1/Lib/shaderc_util.lib
	 C:/VulkanSDK/1.3.231.1/Lib/shaderc_shared.lib
	 C:/VulkanSDK/1.3.231.1/Lib/shaderc.lib
	opengl32
)



