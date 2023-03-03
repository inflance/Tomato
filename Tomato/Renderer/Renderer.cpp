#include "Renderer.hpp"

#include "Renderer2D.hpp"
#include "Tomato/Core/Timer.h"
#include "ShaderFactory.hpp"
#include "RendererConfig.hpp"

namespace Tomato
{
	std::shared_ptr<Renderer::SceneData> Renderer::m_scene_data = std::make_shared<SceneData>();


	void Renderer::EndScene()
	{
	}

	void Renderer::RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline,
	                          Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, uint32_t index_count)
	{
		s_renderer_api_->RenderQuad(renderCommandBuffer, pipeline, vertexBuffer, indexBuffer, index_count);
	}

	void Renderer::RenderBaseShape(Mesh& mesh, const glm::mat4& ViewProjection, const glm::mat4& transform)
	{
	}

	/*void Renderer::RenderBaseShape(BaseShape& baseShap, const glm::vec4& color, const glm::mat4& ViewProjection,
		const glm::mat4& transform)
	{
		const auto& shader = ShaderFactory::Get().GetShader("BaseCube");
		const auto& vertexArray = baseShap.GetVertexArray();
		baseShap.SetData();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", ViewProjection);
		shader->SetMat4("u_Model", transform);
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_Material.Ambient", { 0.2, 0.2, 0.2 });
		shader->SetFloat3("u_Material.Diffuse", { 0.7, 0.7, 0.7 });
		shader->SetFloat3("u_Material.Specular", { 0.7, 0.7, 0.7 });
		shader->SetFloat1("u_Material.Shininess", 32);

		s_renderer_api_->DrawArray(vertexArray, 36);
	}

	void Renderer::RenderBaseShapeWithMatirial(BaseShape& baseShap, const glm::vec4& color, Matirial& matirial,
		const glm::mat4& ViewProjection, const glm::mat4& transform)
	{
		const auto& shader = ShaderFactory::Get().GetShader("BaseCube");
		const auto& vertexArray = baseShap.GetVertexArray();
		baseShap.SetData();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", ViewProjection);
		shader->SetMat4("u_Model", transform);
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_Material.Ambient", matirial.GetAmbient());
		shader->SetInt("u_Material.Diffuse", 0);
		shader->SetInt("u_Material.Specular", 1);
		shader->SetFloat1("u_Material.Shininess", matirial.GetShininess());
		matirial.GetDiffuseMap().Texture->BindUnit(0);
		matirial.GetSpecularMap().Texture->BindUnit(1);

		s_renderer_api_->DrawArray(vertexArray, 36);
		shader->SetInt("u_DirLightSize", 0);
		shader->SetInt("u_PointLightSize", 0);
	}

	void Renderer::RenderBaseLight(BaseShape& baseShap, Light& baseLight, const glm::mat4& ViewProjection,
		const glm::mat4& transform, const glm::vec3& lightPos, int pointLightSize)
	{
		static int pointLightCount = 0;
		if (pointLightCount > pointLightSize - 1)
		{
			pointLightCount = 0;
		}
		const auto& shader = ShaderFactory::Get().GetShader("BaseLight");
		const auto& vertexArray = baseShap.GetVertexArray();
		baseShap.SetData();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", ViewProjection);
		shader->SetMat4("u_Model", transform);
		shader->SetFloat3("u_Color", baseLight.GetColor());

		s_renderer_api_->DrawArray(vertexArray, 36);
		const auto& lightshader = ShaderFactory::Get().GetShader("BaseCube");
		lightshader->Bind();
		switch (baseLight.GetLightType())
		{
		case LightType::DirectionLight:
			lightshader->SetInt("u_DirLightSize", 1);
			lightshader->SetFloat3("u_DirLight.Color", baseLight.GetColor());
			lightshader->SetFloat3("u_DirLight.Direction", baseLight.GetDirection());
			lightshader->SetFloat1("u_DirLight.Intensity", baseLight.GetIntensity());
			break;
		case LightType::PointLight:
			lightshader->SetInt("u_PointLightSize", pointLightSize);
			lightshader->SetFloat3("u_PointLights[" + std::to_string(pointLightCount) + "].Color",
				baseLight.GetColor());
			lightshader->SetFloat3("u_PointLights[" + std::to_string(pointLightCount) + "].Position", lightPos);
			lightshader->SetFloat1("u_PointLights[" + std::to_string(pointLightCount) + "].Intensity",
				baseLight.GetIntensity());
			lightshader->SetFloat1("u_PointLights[" + std::to_string(pointLightCount) + "].Constant",
				baseLight.GetConstant());
			lightshader->SetFloat1("u_PointLights[" + std::to_string(pointLightCount) + "].Linear",
				baseLight.GetLinear());
			lightshader->SetFloat1("u_PointLights[" + std::to_string(pointLightCount) + "].Quadratic",
				baseLight.GetQuadratic());
			++pointLightCount;
			break;
		default:
			break;
		}
	}*/

	void Renderer::OnWindowResize(int x, int y, uint32_t width, uint32_t height)
	{
		s_renderer_api_->SetViewPort(x, y, width, height);
	}

	//uint32_t Renderer::GetCurrentFrameIndex()
	//{
	//	return VulkanContext::GetCurrentSwapChain()->GetCurrentBufferIndex();
	//}

	void Renderer::WaitAndRender()
	{
		Begin();
		//LOG_INFO("Renderer::WaitAndRender");
		s_SubmitQueue.Execute();
		End();
	}

	//render
	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray,
	                      glm::mat4 transform)
	{
		shader->Bind();

		/*	vertexArray->Bind();
			s_renderer_api_->DrawIndexed(vertexArray);*/
	}

	CommandQueue& Renderer::GetRenderResourceReleaseQueue(uint32_t index)
	{
		LOG_ASSERT(index < 3, "");
		return s_ResourceFreeQueue[index];
	}

	void Renderer::Init(Ref<GraphicsContext> context)
	{
		s_renderer_api_->Init(context);
	}

	void Renderer::Destroy()
	{
		LOG_INFO("Renderer Destroy");
		for (int i = 0; i < RendererConfig::MaxFrameInFlight; i++)
		{
			GetRenderResourceReleaseQueue(i).Execute();
		}
	}

	auto Renderer::CreateWhiteTexture()
	{
		
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> renderPass)
	{
		s_renderer_api_->BeginRenderPass(renderPass);
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, Ref<FrameBuffer> frame_buffer)
	{
		s_renderer_api_->BeginRenderPass(renderPass, frame_buffer);
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
