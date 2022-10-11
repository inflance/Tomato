#pragma once

#include "Tomato/Renderer/RendererAPI.h"

namespace Tomato {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();

		void SetViewPort(int x, int y, uint32_t width, uint32_t height);
		void Clear();
		void SetColor(const glm::vec4& color);
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		void Init();
	private:

	};
}