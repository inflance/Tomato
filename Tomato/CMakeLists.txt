# CMakeList.txt: Tomato 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
project("Tomato")

add_definitions(-DTMT_PLATFORM_WINDOWS)

file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

file(GLOB_RECURSE ALL_HEAD "*.hpp" "*.h")

add_library(Tomato  ${ALL_SOURCE} ${ALL_HEAD})

target_precompile_headers(
	${PROJECT_NAME}
	PUBLIC 
	tmtpch.h
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
	opengl32
)
target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}/
	PUBLIC ${ENTT_DIR}/single_include/
	PUBLIC ${ENTT_DIR}/include/
	PUBLIC ${JSON_DIR}/single_include
	PUBLIC ${IMGUIZMO_DIR}
)
# TODO: 如有需要，请添加测试并安装目标。
