cmake_minimum_required(VERSION 3.8)

project(glad C)

add_library(glad "include/glad/glad.h" "include/KHR/khrplatform.h" "src/glad.c")

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${GLAD_DIR}/include
)
target_link_directories(
	${PROJECT_NAME}
	PUBLIC ${GLAD_DIR}/include
)


