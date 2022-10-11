#include "Layer2D.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Tomato/Timer.h"

Layer2D::Layer2D()
	:Layer("Layer2D"), m_cameraControler(1600.0f / 800.0f)
{

}

void Layer2D::OnAttach()
{
	m_texture = Tomato::Texture2D::Create("C:/Users/liyunlo2000/source/repos/Tomato/Tomato/Precompile/Image/DefaultTexture.png");
	//m_square_shader = Tomato::Shader::Create("D:/Dev/cmake/Tomato/Tomato/precompile/square.vert", "D:/Dev/cmake/Tomato/Tomato/precompile/square.frag");

}

void Layer2D::OnDetach()
{

}

void Layer2D::OnUpdate(Tomato::TimeSpan ts)
{

	Tomato::Timer timer("layer2D::OnUpdate");
	
	m_cameraControler.OnUpdate(ts);

	Tomato::Renderer2D::ResetStats();

	static float rotation = 0;
	rotation += float(ts) * 50.0f ;
	Tomato::RendererCommand::SetClearColor(glm::vec4(0.1f, 0.6f, 0.5f, 1.0f));
	Tomato::RendererCommand::Clear();

	Tomato::Renderer2D::BeginScene(m_cameraControler.GetCamera());
	Tomato::Renderer2D::DrawQuad({ 0.3f, 0.0f }, rotation, { 1.0f, 1.0f }, { 0.12f, 0.0f, 0.2f, 1.0f });
	Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.12f, 0.0f, 0.2f, 1.0f });
	Tomato::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 1.0f }, { 0.8f, 0.0f, 0.2f, 1.0f });
	Tomato::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_texture, 10.0f);
	Tomato::Renderer2D::DrawQuad({ -0.5f, 0.0f, 0.1f }, -45.0f, { 0.5f, 0.5f }, m_texture, 1.0f);
	Tomato::Renderer2D::EndScene();

	Tomato::Renderer2D::BeginScene(m_cameraControler.GetCamera());
	for (float i = -5.0f; i < 5.0f; i+=0.5f)
	{
		for (float j = -5.0f; j < 5.0f; j+=0.5f)
		{
			glm::vec4 color = {(i + 5.0f)/10.0f,(j + 5.0f) / 10.0f , (j+i+10.0f) / 20.0f, 0.7f};
			Tomato::Renderer2D::DrawQuad({ i, j, 0.0f }, { 0.45f, 0.45f }, color);
		}
	}
	Tomato::Renderer2D::EndScene();
}

void Layer2D::OnImGuiRender()
{
	ImGui::Begin("setting");
	ImGui::ColorEdit4("triangle color", glm::value_ptr(m_color));

	auto & stats = Tomato::Renderer2D::GetStats();
	ImGui::Text("DrawCalls:%d ", stats.DrawCalls);
	ImGui::Text("Quad: %d", stats.QuadCount);
	ImGui::Text("TotalIndex: %d", stats.GetTotalIndexCount());
	ImGui::Text("TotalVertex: %d", stats.GetTotalVetexCount());


	ImGui::Text("fps: %u", Tomato::TomatoEngine::GetInstance().GetFPS());

	
	ImGui::Text("123" );
	ImGui::End();
}

void Layer2D::OnEvent(Tomato::Event& event)
{
	m_cameraControler.OnEvent(event);
}

