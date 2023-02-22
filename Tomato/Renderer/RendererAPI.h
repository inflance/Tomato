#pragma once

#include <glm/glm.hpp>

#include "GraphicsContext.h"
#include "IndexBuffer.h"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class RenderPass;
	class CommandBuffer;
	class VertexBuffer;
	class Pipeline;

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

		virtual void Init(Ref<GraphicsContext> context) = 0;
		virtual void Clear() = 0;
		virtual void BeginRenderPass() =0;
		virtual void BeginRenderPass(Ref<RenderPass> renderPass) = 0;
		virtual void EndRenderPass() const = 0;
		virtual void Begin() = 0;
		virtual void End() const = 0;
		virtual void SetViewPort(int x, int y, uint32_t width, uint32_t height) = 0;
		virtual void SetColor(const glm::vec4& color) = 0;

		virtual void RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline,
		                        Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer) = 0;
		static API GetCurrentAPI() { return s_api; }
		static void SetCurrentAPI(API api) { s_api = api; }
		static Ref<RendererAPI> Create();

	private:
		static API s_api;
	};
}
