#pragma once

#include "CommandQueue.hpp"
#include "RendererAPI.hpp"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	class Model;
	class IndexBuffer;
	class CommandBuffer;
	class GraphicsContext;
	class FrameBuffer;
	class VertexBuffer;
	class RenderPass;
	class Pipeline;

	class Renderer
	{
	public:
		static void Init(Ref<GraphicsContext> context);
		static void Destroy();

		static void Begin();
		static void End();

		static void BeginRenderPass();
		static void BeginRenderPass(Ref<RenderPass> render_pass);
		static void BeginRenderPass(Ref<RenderPass> render_pass, Ref<FrameBuffer> frame_buffer);
		static void EndRenderPass();

		static void RenderIndexed(Ref<CommandBuffer> render_command_buffer, Ref<Pipeline> pipeline,
		                          Ref<VertexBuffer> vertex_buffer, Ref<IndexBuffer> index_buffer,
		                          uint32_t index_count = 0);

		static void RenderModel(Ref<CommandBuffer> render_command_buffer, Ref<Pipeline> pipeline,const Model& model);
		static void OnWindowResize(int x, int y, uint32_t width, uint32_t height);

		template <typename T>
		static void Submit(T&& func)
		{
			s_SubmitQueue.Allocate(func);
		}

		static void WaitAndRender();

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetCurrentAPI(); }

	private:
		inline static CommandQueue s_SubmitQueue;
		inline static Ref<RendererAPI> s_renderer_api_ = RendererAPI::Create();
	};
}
