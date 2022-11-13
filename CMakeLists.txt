# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project (TomatoEditor)

add_executable(${PROJECT_NAME}  "Main.cpp" )

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(GLFW_DIR ${ENGINE_DIR}/thirdparty/glfw)
set(GLAD_DIR ${ENGINE_DIR}/thirdparty/glad)
set(IMGUI_DIR ${ENGINE_DIR}/thirdparty/imgui)
set(GLM_DIR ${ENGINE_DIR}/thirdparty/glm)
set(STB_DIR ${ENGINE_DIR}/thirdparty/stb)
set(ENTT_DIR ${ENGINE_DIR}/thirdparty/entt)
set(JSON_DIR ${ENGINE_DIR}/thirdparty/json)
set(IMGUIZMO_DIR ${ENGINE_DIR}/thirdparty/ImGuizmo)

add_definitions(-DTMT_PLATFORM_WINDOWS)



add_subdirectory(Tomato)
add_subdirectory(Editor)
add_subdirectory(thirdparty)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} 
	Tomato
	Editor
)




