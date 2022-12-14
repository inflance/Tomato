#pragma once

#include "RendererAPI.h"

namespace Tomato {

	class RendererCommand
	{
	public:
		inline static void Init() {
			s_renderer_api->Init();
		}
		inline static void SetViewPort(int x, int y, uint32_t width, uint32_t height) {
			s_renderer_api->SetViewPort(x, y, width , height);
		}
		inline static void SetClearColor(const glm::vec4& color) {
			s_renderer_api->SetColor(color);
		}
		inline static void Clear() {
			s_renderer_api->Clear();
		}
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) {
			s_renderer_api->DrawIndexed(vertexArray, count);
		}
		inline static void DrawArray(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0) {
			s_renderer_api->DrawArray(vertexArray, count);
		}
	private:
		static std::shared_ptr<RendererAPI> s_renderer_api;
	};
}