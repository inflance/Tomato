#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Tomato {
	
	class RendererAPI 
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			Vulkan,
			DirectX12,
		};

	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void SetViewPort(int x, int y, uint32_t width, uint32_t height) = 0;

		inline static API GetAPI() { return s_api; };
	private:
		static API s_api;

	};

}