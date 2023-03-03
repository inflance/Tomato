#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "GraphicsContext.hpp"
//#include "OrthographicCamera.h"
#include "Tomato/Scene/Light.hpp"
#include "Tomato/Core/Engine.h"
#include  "Pipeline.hpp"
#include "CommandQueue.hpp"
#include "RendererAPI.hpp"
#include "Vulkan/VulkanContext.hpp"
#include "Vulkan/VulkanPipeline.hpp"

namespace Tomato
{
	class FrameBuffer;

	class Renderer
	{
	public:
		//≥ı ºªØ‰÷»æ∆˜
		static void Init(Ref<GraphicsContext> context);
		static void Destroy();

		static auto CreateWhiteTexture();
		static void BeginRenderPass();
		static void BeginRenderPass(Ref<RenderPass> renderPass);
		static void BeginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frame_buffer);
		static void EndRenderPass();
		static void Begin();
		static void End();
		//static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline,
		                       Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, uint32_t index_count = 0);
		
		//‰÷»ææ≤Ã¨Õ¯∏Ò
		static void RenderBaseShape(Mesh& mesh, const glm::mat4& ViewProjection, const glm::mat4& transform);
		/*	static void RenderBaseShapeWithMatirial(BaseShape& baseShap, const glm::vec4& color, Matirial& matirial,
				const glm::mat4& ViewProjection, const glm::mat4& transform);
			static void RenderBaseShape(BaseShape& baseShap, const glm::vec4& color, const glm::mat4& ViewProjection,
				const glm::mat4& transform);
			static void RenderBaseLight(BaseShape& baseShap, Light& baseLight, const glm::mat4& ViewProjection,
				const glm::mat4& transform, const glm::vec3& lightPos, int pointLightSize);*/

		static void OnWindowResize(int x, int y, uint32_t width, uint32_t height);
		static void Submit(std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray,
		                   glm::mat4 transform = glm::mat4(1.0f));

		template <typename T>
		static void Submit(T&& func)
		{
			s_SubmitQueue.Allocate(func);
		}

		template <typename T>
		static void SubmitAndFree(T&& func)
		{
			//auto index = VulkanContext::GetCurrentSwapChain()->GetCurrentBufferIndex();
			//GetRenderResourceReleaseQueue(index).Allocate(func);
		}

		static uint32_t GetCurrentFrameIndex();

		static void WaitAndRender();

		static void Submit(const std::shared_ptr<Pipeline>& pipeline);

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetCurrentAPI(); }
		static CommandQueue& GetRenderResourceReleaseQueue(uint32_t uint32);
		//static void RegisterShaderDependency(const std::shared_ptr<Shader>& shared, VulkanPipeline* vulkan_pipeline);

	private:
		inline static CommandQueue s_ResourceFreeQueue[3];
		inline static CommandQueue s_SubmitQueue;
		inline static Ref<RendererAPI> s_renderer_api_ = RendererAPI::Create();

		struct SceneData
		{
			glm::mat4 ViewProjection;
		};

		static std::shared_ptr<SceneData> m_scene_data;
	};
}
