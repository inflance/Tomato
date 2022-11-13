#include "Renderer.h"

#include "Tomato/Platform/OpenGL/OpenGLShader.h"

#include "Renderer2D.h"

namespace Tomato {

	Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData;

	//设置相机，视角
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_scene_data->ViewProjection = camera.GetViewProjectionMat();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::OnWindowResize(int x, int y, uint32_t width, uint32_t height)
	{
		//RendererCommand::SetViewPort(x, y, width, height);
	}

	//render
	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, glm::mat4 transform )
	{
		shader->Bind();
		
		//设置视角投影矩阵
		std::dynamic_pointer_cast<Tomato::OpenGLShader>(shader)->SetMat4("m_viewProjection", m_scene_data->ViewProjection);
		//设置变换矩阵
		std::dynamic_pointer_cast<Tomato::OpenGLShader>(shader)->SetMat4("m_transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Init()
	{
		RendererCommand::Init();
		Renderer2D::Init();
	}

}