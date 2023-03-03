#pragma once

#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/GraphicsContext.hpp"
#include "Tomato/Renderer/Pipeline.hpp"
#include "Tomato/Renderer/RendererAPI.hpp"
#include "Vulkan.hpp"

namespace Tomato
{
	class VulkanContext;

	class VulkanRendererAPI final : public RendererAPI
	{
	public:
		VulkanRendererAPI();

		void Init(Ref<GraphicsContext> context) override;
		void BeginRenderPass() override;
		void BeginRenderPass(Ref<RenderPass> renderPass) override;
		void BeginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frame_buffer) override;
		void EndRenderPass() const override;
		void RenderIndexed(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline, Ref<VertexBuffer> vertexBuffer,
		                Ref<IndexBuffer> indexBuffer, uint32_t index_count = 0) override;

		void SetViewPort(int x, int y, uint32_t width, uint32_t height) override;
		void Clear() override;
		void SetColor(const glm::vec4& color) override;

		void Begin() override;

		void End() const override;
		void RenderModel(const std::shared_ptr<CommandBuffer>& ref, const std::shared_ptr<Pipeline>& shared,
			const Model& model) override;

	private:
		Ref<VulkanContext> m_context_;
		vk::Rect2D m_rect2d;
		vk::Viewport m_viewport;
	};
}
