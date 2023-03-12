#include "Renderer.hpp"

#include "GraphicsContext.hpp"
#include "Renderer2D.hpp"
#include "CommandBuffer.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Model.hpp"

namespace Tomato
{
	void Renderer::Init(Ref<GraphicsContext> context)
	{
		LOG_TRACE("Renderer API Init");
		s_renderer_api_->Init(context);
		Renderer2D::Init();
	}

	void Renderer::Destroy()
	{
		Renderer2D::Shutdown();
		LOG_TRACE("Renderer API Destroy");
	}


	void Renderer::RenderIndexed(Ref<CommandBuffer> render_command_buffer, Ref<Pipeline> pipeline,
		Ref<VertexBuffer> vertex_buffer, Ref<IndexBuffer> index_buffer, uint32_t index_count)
	{
		s_renderer_api_->RenderIndexed(render_command_buffer, pipeline, vertex_buffer, index_buffer, index_count);
	}

	void Renderer::RenderModel(Ref<CommandBuffer> render_command_buffer, Ref<Pipeline> pipeline, const  Model& model)
	{
		s_renderer_api_->RenderModel(render_command_buffer, pipeline, model);
	}


	void Renderer::OnWindowResize(int x, int y, uint32_t width, uint32_t height)
	{
		s_renderer_api_->SetViewPort(x, y, width, height);
	}

	void Renderer::WaitAndRender()
	{
		Begin();
		s_SubmitQueue.Execute();
		End();
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> render_pass)
	{
		s_renderer_api_->BeginRenderPass(render_pass);
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> render_pass, Ref<FrameBuffer> frame_buffer)
	{
		s_renderer_api_->BeginRenderPass(render_pass, frame_buffer);
	}

	void Renderer::BeginRenderPass()
	{
		s_renderer_api_->BeginRenderPass();
	}

	void Renderer::EndRenderPass()
	{
		s_renderer_api_->EndRenderPass();
	}

	void Renderer::Begin()
	{
		s_renderer_api_->Begin();
	}

	void Renderer::End()
	{
		s_renderer_api_->End();
	}
}
