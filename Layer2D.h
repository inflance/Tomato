#pragma once

#include "Tomato/Tomato.h"
#include "Tomato/ParticleSystem.h"

class Layer2D : public Tomato::Layer
{
public:
	Layer2D();
	virtual ~Layer2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void Tick(Tomato::TimeSpan ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Tomato::Event& event) override;

private:
	Tomato::OrthgraphicCameraControler m_cameraControler;

	std::shared_ptr<Tomato::Shader> m_square_shader;
	std::shared_ptr<Tomato::VertexArray> m_square_vertex_array;
	std::shared_ptr<Tomato::Texture2D> m_texture;
	std::shared_ptr<Tomato::Texture2D> m_texture1;
	std::shared_ptr<Tomato::SubTexture2D> m_subtexture;
	std::shared_ptr<Tomato::IFrameBuffer> m_frameBuffer;

	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	glm::vec2 m_viewPortSize = glm::vec2(0);

	bool m_viewPortFocused = false;
	bool m_viewPortHovered = false;

	bool firstEnter = true;

	float m_timeSpan;
	uint32_t m_count = 0;

	//Tomato::ParticleSystem m_particleSystem;
	Tomato::ParticleProps m_particle;
};