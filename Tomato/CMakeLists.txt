# CMakeList.txt: Tomato 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
project(Tomato)

add_definitions(-DTMT_PLATFORM_WINDOWS)

add_subdirectory(Renderer "./Renderer")

add_library(Tomato  "Engine.h" "Engine.cpp" "LogSystem.h" "LogSystem.cpp" "Events/Event.h" "Events/KeyEvent.h" "Events/ApplicatiOnEvent.h" "Events/MouseEvent.h" "Window.h" "Platform/Windows/WindowsWindow.h" "Platform/Windows/WindowsWindow.cpp" "Window.cpp" "Core.h" "Layer.h" "Layer.cpp" "LayerStack.h" "LayerStack.cpp" "Tomato.h" "ImGui/ImGuiLayer.h" "ImGui/ImGuiLayer.cpp"  "Input.h" "Platform/Windows/WindowsInput.cpp"  "Platform/OpenGL/OpenGLContext.h" "Platform/OpenGL/OpenGLContext.cpp" "Renderer/GraphicsContext.h" "Renderer/Shader.h" "Renderer/Shader.cpp" "Renderer/Buffer.h" "Renderer/Buffer.cpp" "Platform/OpenGL/OpenGLBuffer.h" "Platform/OpenGL/OpenGLBuffer.cpp" "Renderer/Renderer.h" "Renderer/Renderer.cpp" "Renderer/VertexArray.h" "Renderer/VertexArray.cpp" "Platform/OpenGL/OpenGLVertexArray.h" "Platform/OpenGL/OpenGLVertexArray.cpp" "Renderer/RendererAPI.h" "Renderer/RendererAPI.cpp" "Renderer/RendererCommand.h" "Renderer/RendererCommand.cpp"  "Platform/OpenGL/OpenGLRendererAPI.h" "Platform/OpenGL/OpenGLRendererAPI.cpp" "OrthographicCameraControler.h" "Renderer/OrthographicCamera.cpp"   "KeyCode.h" "TimeSpan.h" "Platform/OpenGL/OpenGLShader.h" "Platform/OpenGL/OpenGLShader.cpp" "Renderer/Texture.h" "Renderer/Texture.cpp" "Macro.h" "Platform/OpenGL/OpenGLTexture.h" "Platform/OpenGL/OpenGLTexture.cpp" "Renderer/ShaderFactory.h" "Renderer/ShaderFactory.cpp" "OrthographicCameraControler.h" "OrthographicCameraControler.cpp" "Renderer/Renderer2D.h" "Renderer/Renderer2D.cpp"   "Timer.h" "Light.h"  "GameLayer.cpp")

target_include_directories(${PROJECT_NAME}
	PUBLIC ${ENGINE_DIR}
	PUBLIC ${GLFW_DIR}include
	PUBLIC ${SPDLOG_DIR}include
	PUBLIC ${GLAD_DIR}include
	PUBLIC ${IMGUI_DIR}
	PUBLIC ${GLM_DIR} 
	PUBLIC ${STB_DIR} 
)

target_link_directories(${PROJECT_NAME}
	PUBLIC ${GLFW_DIR}src
	PUBLIC ${SPDLOG_DIR}src
	PUBLIC ${GLAD_DIR}src
	PUBLIC ${IMGUI_DIR}
	PUBLIC ${GLM_DIR} 
	PUBLIC ${STB_DIR} 
)

target_precompile_headers(
	${PROJECT_NAME}
	PUBLIC 
	tmtpch.h
)

target_link_libraries(${PROJECT_NAME} 
	glfw
	spdlog
	glad
	imgui
	glm
	stb
	opengl32
)

# TODO: 如有需要，请添加测试并安装目标。
