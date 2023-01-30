#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "BaseShape.h"
#include "RendererCommand.h"
#include "GraphicsContext.h"
#include "OrthographicCamera.h"
#include "Tomato/Scene/Light.h"
#include "Tomato/Core/Engine.h"
#include  "Pipeline.h"
#include "CommandQueue.h"
#include "Vulkan/VulkanPipeline.h"

namespace Tomato
{

	class Renderer
	{
	public:
		struct Config
		{
			uint32_t max_frame = 3;
		};
		inline static Config s_renderer_config;
		//≥ı ºªØ‰÷»æ∆˜
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		//‰÷»ææ≤Ã¨Õ¯∏Ò
		static void RenderBaseShape(Mesh& mesh, const glm::mat4& ViewProjection, const glm::mat4& transform);
		static void RenderBaseShapeWithMatirial(BaseShape& baseShap, const glm::vec4& color, Matirial& matirial,
		                                        const glm::mat4& ViewProjection, const glm::mat4& transform);
		static void RenderBaseShape(BaseShape& baseShap, const glm::vec4& color, const glm::mat4& ViewProjection,
		                            const glm::mat4& transform);
		static void RenderBaseLight(BaseShape& baseShap, Light& baseLight, const glm::mat4& ViewProjection,
		                            const glm::mat4& transform, const glm::vec3& lightPos, int pointLightSize);

		static void OnWindowResize(int x, int y, uint32_t width, uint32_t height);
		static void Submit(std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray,
		                   glm::mat4 transform = glm::mat4(1.0f));

		template<typename T>
		static void Submit(T&& func)
		{
			s_SubmitQueue.Allocate(func);
		}
		template<typename T>
		static void SubmitAndFree(T&& func)
		{
			auto index = 3;
			GetRenderResourceReleaseQueue(index).Allocate(func);
		}

		static void Submit(const std::shared_ptr<Pipeline>& pipeline);

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetCurrentAPI(); }
		static CommandQueue& GetRenderResourceReleaseQueue(uint32_t uint32);
		static void RegisterShaderDependency(const std::shared_ptr<Shader>& shared, VulkanPipeline* vulkan_pipeline);

	private:
		inline static CommandQueue s_ResourceFreeQueue[3];
		inline static CommandQueue s_SubmitQueue;
		struct SceneData
		{
			glm::mat4 ViewProjection;
		};

		static std::shared_ptr<SceneData> m_scene_data;
	};
}
