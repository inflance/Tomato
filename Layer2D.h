#pragma once

#include "Tomato/Tomato.h"

class Layer2D : public Tomato::Layer
{
public:
	Layer2D();
	virtual ~Layer2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Tomato::TimeSpan ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Tomato::Event& event) override;

private:
	Tomato::OrthgraphicCameraControler m_cameraControler;

	std::shared_ptr<Tomato::Shader> m_square_shader;
	std::shared_ptr<Tomato::VertexArray> m_square_vertex_array;
	std::shared_ptr<Tomato::Texture2D> m_texture;
	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	float m_timeSpan;
	uint32_t m_count = 0;

	struct ProfileResult 
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_profileResults;
};