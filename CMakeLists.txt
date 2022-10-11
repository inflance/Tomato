# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("TomatoEditor")

add_executable(${PROJECT_NAME} "Main.cpp"  "Layer2D.h" "Layer2D.cpp" "GameLayer.h")

set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR})



add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/imgui)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/stb)
add_subdirectory(Tomato)


target_link_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} 
	Tomato
)




