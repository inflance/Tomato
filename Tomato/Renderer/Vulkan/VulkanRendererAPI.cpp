#include "VulkanRendererAPI.hpp"

#include "VulkanContext.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanPipeline.hpp"

namespace Tomato
{
	VulkanRendererAPI::VulkanRendererAPI()
	{
	}

	void VulkanRendererAPI::Init(Ref<GraphicsContext> context)
	{
		m_context_ = As<VulkanContext>(context);
	}

	void VulkanRendererAPI::BeginRenderPass(Ref<RenderPass> renderPass)
	{
		const auto& render_pass = As<VulkanRenderPass>(renderPass);
		const auto& clearColor = render_pass->GetProps().clear_value.clear_color_;
		std::array<vk::ClearValue, 2> clearValues;
		clearValues[0].color = vk::ClearColorValue(std::array{clearColor.x, clearColor.y, clearColor.z, clearColor.w});
		clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
		const vk::RenderPassBeginInfo renderPassBeginInfo(*render_pass->renderPass,
		                                                  *m_context_->GetCurrentFrameBuffer(),
		                                                  m_rect2d, clearValues);
		m_context_->GetCurrentCommandBuffer().beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

	void VulkanRendererAPI::BeginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frame_buffer)
	{
		const auto& render_pass = As<VulkanRenderPass>(renderPass);
		const auto& clearColor = render_pass->GetProps().clear_value.clear_color_;
		const auto ins = this;
		Renderer::Submit([clearColor, render_pass, frame_buffer, ins]
		{
			std::array<vk::ClearValue, 2> clearValues;
			clearValues[0].color = vk::ClearColorValue(std::array{
				clearColor.x, clearColor.y, clearColor.z, clearColor.w
			});
			clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
			const vk::RenderPassBeginInfo renderPassBeginInfo(*render_pass->renderPass,
			                                                  *As<VulkanFrameBuffer>(frame_buffer)->m_framebuffer,
			                                                  vk::Rect2D({0, 0}, {
				                                                             frame_buffer->GetFrameBufferInfo().width_,
				                                                             frame_buffer->GetFrameBufferInfo().height_
			                                                             }), clearValues);
			ins->m_context_->GetCurrentCommandBuffer().beginRenderPass(renderPassBeginInfo,
			                                                           vk::SubpassContents::eInline);

			ins->m_context_->GetCurrentCommandBuffer().setViewport(
				0, vk::Viewport(0, 0, frame_buffer->GetFrameBufferInfo().width_,
				                frame_buffer->GetFrameBufferInfo().height_,
				                0, 1));
			ins->m_context_->GetCurrentCommandBuffer().setScissor(0, vk::Rect2D({0, 0}, {
				                                                      frame_buffer->GetFrameBufferInfo().
				                                                      width_,
				                                                      frame_buffer->GetFrameBufferInfo().
				                                                      height_
			                                                      }));
		});
	}

	void VulkanRendererAPI::BeginRenderPass()
	{
		const auto ins = this;
		Renderer::Submit([ins]
		{
			const auto& render_pass = VulkanContext::Get().renderPass;
			std::array<vk::ClearValue, 2> clearValues;
			clearValues[0].color = vk::ClearColorValue(std::array{0, 0, 0, 0});
			clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
			const vk::RenderPassBeginInfo renderPassBeginInfo(*render_pass,
			                                                  *ins->m_context_->GetCurrentFrameBuffer(),
			                                                  ins->m_rect2d, clearValues);
			ins->m_context_->GetCurrentCommandBuffer().beginRenderPass(renderPassBeginInfo,
			                                                           vk::SubpassContents::eInline);
		});
	}

	void VulkanRendererAPI::EndRenderPass() const
	{
		const auto ins = this;
		Renderer::Submit([ ins]
		{
			ins->m_context_->GetCurrentCommandBuffer().endRenderPass();
		});
	}

	void VulkanRendererAPI::RenderIndexed(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline,
	                                      Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer,
	                                      uint32_t index_count)
	{
		const auto& graphics_pipeline = As<VulkanPipeline>(pipeline);
		const auto& vertex_buffer = As<VulkanVertexBuffer>(vertexBuffer);
		const auto& index_buffer = As<VulkanIndexBuffer>(indexBuffer);

		if (index_count == 0) index_count = index_buffer->GetCount();
		const auto ins = this;
		Renderer::Submit([index_count, graphics_pipeline, vertex_buffer, index_buffer, ins]
		{
			ins->m_context_->GetCurrentCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics,
			                                                        *graphics_pipeline->graphicsPipeline);
			ins->m_context_->GetCurrentCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
			                                                              *graphics_pipeline->pipelineLayout, 0, {
				                                                              *graphics_pipeline->descriptorSet.front()
			                                                              },
			                                                              nullptr);
			ins->m_context_->GetCurrentCommandBuffer().bindVertexBuffers(0, {*vertex_buffer->buffer}, {0});
			ins->m_context_->GetCurrentCommandBuffer().bindIndexBuffer(*index_buffer->m_index_buffer, 0,
			                                                           vk::IndexType::eUint32);
			ins->m_context_->GetCurrentCommandBuffer().setViewport(
				0, ins->m_viewport);
			ins->m_context_->GetCurrentCommandBuffer().setScissor(0, ins->m_rect2d);

			ins->m_context_->GetCurrentCommandBuffer().drawIndexed(index_count, 1, 0, 0, 0);
		});
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


	void VulkanRendererAPI::Begin()
	{
		m_context_->GetCurrentCommandBuffer().begin({});
	}

	void VulkanRendererAPI::End() const
	{
		m_context_->GetCurrentCommandBuffer().end();
	}

	void VulkanRendererAPI::RenderModel(const std::shared_ptr<CommandBuffer>& ref,
	                                    const std::shared_ptr<Pipeline>& shared, const Model& model)
	{
	}
}
