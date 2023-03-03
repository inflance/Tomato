cmake_minimum_required (VERSION 3.8)

project (Editor)

add_library(Editor  "LevelPanel.hpp"  "AssetPanel.h" "AssetPanel.cpp"  "ViewPortPanel.hpp"  "ViewPortPanel.cpp" "LevelPanel.cpp" )

target_link_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} 
	Tomato
)