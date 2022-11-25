#pragma once

#include "Tomato/Tomato.h"
#include "Tomato/ParticleSystem.h"
#include "ScenePanel.h"
#include "Tomato/Scene/SceneSerializater.h"

namespace Tomato{

	enum class EditorMode {
		DefaultMode = 0,
		CameraMode,
		GizmoMode
	};

	class Editor : public Layer
	{
	public:
		Editor();
		virtual ~Editor() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Tick(TimeSpan ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
		bool OnKeyPressed(KeyPressedEvent& e);
		
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		void  CreateNewScene();
		void  OpenScene();
		void  SaveSceneAs();


	private:
		OrthgraphicCameraControler m_cameraControler;

		std::shared_ptr<Shader> m_square_shader;
		std::shared_ptr<VertexArray> m_square_vertex_array;
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<Texture2D> m_texture1;
		std::shared_ptr<SubTexture2D> m_subtexture;
		std::shared_ptr<IFrameBuffer> m_frameBuffer;

		std::shared_ptr<Scene> m_Scene;

		EditorCamera m_editorCamera;

		EditorMode m_editorMode;

		ScenePanel m_ScenePanel;
		Entity m_hoveredEntity;

		int m_zgmoMode = -1;

		bool m_mainCamera = true;

		glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		glm::vec2 m_viewPortSize = glm::vec2(0);

		bool m_viewPortFocused = false;
		bool m_viewPortHovered = false;

		bool firstEnter = true;

		float m_timeSpan;
		uint32_t m_count = 0;
		glm::vec2 m_viewportBounds[2];

		ParticleProps m_particle;
	};
}
