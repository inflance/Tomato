cmake_minimum_required (VERSION 3.8)

project (Editor)

add_library(Editor "Editor.h" "Editor.cpp" "ScenePanel.h" "ScenePanel.cpp" "AssetPanel.h" "AssetPanel.cpp"  )

target_link_directories(
	${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
)

target_link_libraries(
	${PROJECT_NAME} 
	Tomato
)