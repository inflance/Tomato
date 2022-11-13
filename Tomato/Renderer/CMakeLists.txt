# CMakeList.txt: Tomato 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
project(Renderer)

add_library(Renderer "GraphicsContext.h" "Shader.h" "Shader.cpp" "Buffer.h" "Buffer.cpp" "Renderer.h" "Renderer.cpp" "VertexArray.h" "VertexArray.cpp"  "RendererAPI.h" "RendererAPI.cpp" "RendererCommand.h" "RendererCommand.cpp"  "OrthographicCamera.cpp" "Texture.h" "Texture.cpp" "ShaderFactory.h" "ShaderFactory.cpp" "Renderer2D.h" "Renderer2D.cpp")

