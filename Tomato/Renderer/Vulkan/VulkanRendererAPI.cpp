#include "VulkanRendererAPI.h"

#include "VulkanContext.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexBuffer.h"

namespace Tomato
{
	void VulkanRendererAPI::Init(Ref<GraphicsContext> context)
	{
		m_context_ = As<VulkanContext>(context);
	}

	void VulkanRendererAPI::BeginRenderPass(Ref<RenderPass> renderPass)
	{
		const auto& render_pass = As<VulkanRenderPass>(renderPass);
		const auto& clearColor = render_pass->props.ClearValue.ClearColor;
		std::array<vk::ClearValue, 2> clearValues;
		clearValues[0].color = vk::ClearColorValue(std::array{clearColor.x, clearColor.y, clearColor.z, clearColor.w});
		clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
		const vk::RenderPassBeginInfo renderPassBeginInfo(*render_pass->renderPass,
		                                                  *m_context_->GetCurrentFrameBuffer(),
		                                                  m_rect2d, clearValues);
		m_context_->GetCurrentCommandBuffer().beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

	void VulkanRendererAPI::BeginRenderPass()
	{
		const auto& render_pass = VulkanContext::Get().renderPass;
		std::array<vk::ClearValue, 2> clearValues;
		clearValues[0].color = vk::ClearColorValue(std::array{0, 0, 0, 0});
		clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
		const vk::RenderPassBeginInfo renderPassBeginInfo(*render_pass,
		                                                  *m_context_->GetCurrentFrameBuffer(),
		                                                  m_rect2d, clearValues);
		m_context_->GetCurrentCommandBuffer().beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

	void VulkanRendererAPI::EndRenderPass() const
	{
		m_context_->GetCurrentCommandBuffer().endRenderPass();
	}

	void VulkanRendererAPI::RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline,
	                                   Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
	{
		const auto& graphicsPipeline = As<VulkanPipeline>(pipeline);
		const auto& vertex_buffer = As<VulkanVertexBuffer>(vertexBuffer);
		const auto& index_buffer = As<VulkanIndexBuffer>(indexBuffer);
		m_context_->GetCurrentCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics,
		                                                   *graphicsPipeline->graphicsPipeline);
		m_context_->GetCurrentCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
		                                                         *graphicsPipeline->pipelineLayout, 0, {
			                                                         *graphicsPipeline->descriptorSet.front()
		                                                         },
		                                                         nullptr);
		m_context_->GetCurrentCommandBuffer().bindVertexBuffers(0, {*vertex_buffer->buffer}, {0});
		m_context_->GetCurrentCommandBuffer().bindIndexBuffer(*index_buffer->m_index_buffer, 0, vk::IndexType::eUint32);
		m_context_->GetCurrentCommandBuffer().setViewport(
			0, m_viewport);
		m_context_->GetCurrentCommandBuffer().setScissor(0, m_rect2d);

		m_context_->GetCurrentCommandBuffer().drawIndexed(index_buffer->GetCount(), 1, 0, 0, 0);
	}

	void VulkanRendererAPI::SetViewPort(int x, int y, uint32_t width, uint32_t height)
	{
		m_rect2d = vk::Rect2D({x, y}, {width, height});
		m_viewport = vk::Viewport(x, y, width, height, 0.0f, 1.0f);
		m_context_->OnResize(width, height);
	}

	void VulkanRendererAPI::Clear()
	{
	}

	void VulkanRendererAPI::SetColor(const glm::vec4& color)
	{
	}

	//void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count)
	//{
	//}

	//void VulkanRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count)
	//{
	//}

	void VulkanRendererAPI::Begin()
	{
		m_context_->GetCurrentCommandBuffer().begin({});
	}

	void VulkanRendererAPI::End() const
	{
		m_context_->GetCurrentCommandBuffer().end();
	}
}
