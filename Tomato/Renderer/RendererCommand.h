#pragma once

#include "RendererAPI.h"

namespace Tomato
{
	class RendererCommand
	{
	public:
		static void Init()
		{
			s_renderer_api = Create();
			s_renderer_api->Init();
		}

		static void SetViewPort(int x, int y, uint32_t width, uint32_t height)
		{
			s_renderer_api->SetViewPort(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_renderer_api->SetColor(color);
		}

		static void Clear()
		{
			s_renderer_api->Clear();
		}

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_renderer_api->DrawIndexed(vertexArray, count);
		}

		static void DrawArray(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_renderer_api->DrawArray(vertexArray, count);
		}

		static void Submit(const std::shared_ptr<Pipeline>& m_pipeline)
		{
			s_renderer_api->Submit(m_pipeline);
		}

		static std::shared_ptr<RendererAPI> Create();

	private:
		inline static std::shared_ptr<RendererAPI> s_renderer_api = nullptr;
	};
}
