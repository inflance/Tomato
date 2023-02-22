#pragma once

#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/GraphicsContext.h"
#include "Tomato/Renderer/Pipeline.h"
#include "Tomato/Renderer/RendererAPI.h"
#include "Vulkan.h"

namespace Tomato
{
	class VulkanContext;

	class VulkanRendererAPI final : public RendererAPI
	{
	public:
		VulkanRendererAPI() = default;

		void Init(Ref<GraphicsContext> context) override;
		void BeginRenderPass() override;
		void BeginRenderPass(Ref<RenderPass> renderPass) override;
		void EndRenderPass() const override;
		void RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline, Ref<VertexBuffer> vertexBuffer,
		                Ref<IndexBuffer> indexBuffer) override;

		void SetViewPort(int x, int y, uint32_t width, uint32_t height) override;
		void Clear() override;
		void SetColor(const glm::vec4& color) override;
		//void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) override;
		//void DrawArray(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count = 0) override;

		void Begin() override;

		void End() const override;

	private:
		Ref<VulkanContext> m_context_;
		vk::Rect2D m_rect2d;
		vk::Viewport m_viewport;
	};
}
