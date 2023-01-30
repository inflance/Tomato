#include "Renderer.h"

#include "Renderer2D.h"
#include "Tomato/Core/Timer.h"
#include "ShaderFactory.h"

namespace Tomato
{
	std::shared_ptr<Renderer::SceneData> Renderer::m_scene_data = std::make_shared<SceneData>();

	//设置相机，视角
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_scene_data->ViewProjection = camera.GetViewProjectionMat();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::RenderBaseShape(Mesh& mesh, const glm::mat4& ViewProjection, const glm::mat4& transform)
	{
		const auto& shader = ShaderFactory::Get().GetShader("StaticMesh");
		const auto& submeshs = mesh.GetMesh();
		for (auto& sub_mesh : submeshs)
		{
			sub_mesh.Draw(
				[&](auto& vertexArray, auto& vertexBuffer, auto& verties, auto& texture)
				{
					vertexBuffer->SetData(verties.data(), sizeof(Vertex) * verties.size());

					shader->Bind();
					shader->SetMat4("u_ViewProjection", ViewProjection);
					shader->SetMat4("u_Model", transform);
					if (texture.size())
					{
						shader->SetInt("u_Diffuse", 0);
						texture[0].Texture->Bind();
					}
					RendererCommand::DrawIndexed(vertexArray);
				}
			);
		}
	}

	void Renderer::RenderBaseShape(BaseShape& baseShap, const glm::vec4& color, const glm::mat4& ViewProjection,
	                               const glm::mat4& transform)
	{
		const auto& shader = ShaderFactory::Get().GetShader("BaseCube");
		const auto& vertexArray = baseShap.GetVertexArray();
		baseShap.SetData();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", ViewProjection);
		shader->SetMat4("u_Model", transform);
		shader->SetFloat4("u_Color", color);
		shader->SetFloat3("u_Material.Ambient", {0.2, 0.2, 0.2});
		shader->SetFloat3("u_Material.Diffuse", {0.7, 0.7, 0.7});
		shader->SetFloat3("u_Material.Specular", {0.7, 0.7, 0.7});
		shader->SetFloat1("u_Material.Shininess", 32);

		RendererCommand::DrawArray(vertexArray, 36);
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

		RendererCommand::DrawArray(vertexArray, 36);
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

		RendererCommand::DrawArray(vertexArray, 36);
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
	}

	void Renderer::OnWindowResize(int x, int y, uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewPort(x, y, width, height);
	}

	//render
	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray,
	                      glm::mat4 transform)
	{
		shader->Bind();

		////设置视角投影矩阵
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("m_viewProjection", m_scene_data->ViewProjection);
		////设置变换矩阵
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("m_transform", transform);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const std::shared_ptr<Pipeline>& pipeline)
	{
		RendererCommand::Submit(pipeline);
	}

	CommandQueue& Renderer::GetRenderResourceReleaseQueue(uint32_t index)
	{
		LOG_ASSERT(index < 3, "");
		return s_ResourceFreeQueue[index];
	}

	void Renderer::Init()
	{
		RendererCommand::Init();
		Renderer2D::Init();
	}
}
