cmake_minimum_required (VERSION 3.8)

project("Tomato")

add_definitions(-DTMT_PLATFORM_WINDOWS)

file(GLOB_RECURSE ALL_SOURCE "*.cpp" "*.c")

file(GLOB_RECURSE ALL_HEAD "*.hpp" "*.h")

add_library(Tomato  ${ALL_SOURCE} ${ALL_HEAD})

target_precompile_headers(
	${PROJECT_NAME}
	PUBLIC 
	TmtPch.h
)

file(GLOB imguiInitFile "./ImGui/imgui.ini")
file(COPY ${imguiInitFile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
	PUBLIC ${ENTT_DIR}/single_include
	PUBLIC ${ENTT_DIR}/src
	PUBLIC ${JSON_DIR}/single_include
	PUBLIC ${IMGUIZMO_DIR}
)

