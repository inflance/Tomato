#pragma once

#include "ScenePanel.h"
#include "AssetPanel.h"

#include "Tomato/Tomato.h"
#include "Tomato/Renderer/Mesh.h"


namespace Tomato
{
	class Pipeline;

	enum class EditorMode
	{
		DefaultMode = 0,
		CameraMode,
		GizmoMode
	};

	class Editor : public Layer
	{
	public:
		Editor();
		~Editor() override = default;

		void OnCreate() override;
		void OnDestroy() override;

		void Tick(float deltaTime) override;
		void OnImGuiRenderer() override;
		void OnEvent(Event& event) override;

		void CreateNewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void DeBugInfoPanel();

	private:
		OrthgraphicCameraControler m_cameraControler;

		std::shared_ptr<Shader> m_square_shader;
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<Texture2D> m_texture1;
		std::shared_ptr<TextureCube> m_texture2;
		std::shared_ptr<SubTexture2D> m_subtexture;
		std::shared_ptr<Framebuffer> m_frameBuffer;
		std::shared_ptr<Pipeline> m_pipeline;
		std::shared_ptr<CommandBuffer> m_command_buffer_;
		Ref<Shader> m_MeshShader;
		Ref<Shader> m_ShapeShader;
		Ref<Shader> m_LightShader;

		std::shared_ptr<Scene> m_Scene;
		Entity m_hoveredEntity;
		Mesh mesh;

		EditorCamera m_editorCamera;
		EditorMode m_editorMode = EditorMode::DefaultMode;

		ScenePanel m_ScenePanel;
		AssetPanel m_asset_panel;

		int m_zgmoMode = -1;

		bool m_mainCamera = true;

		glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		glm::vec2 m_viewPortSize = glm::vec2(0);
		glm::vec2 m_viewportBounds[2];

		bool m_viewPortFocused = false;
		bool m_viewPortHovered = false;

		bool firstEnter = true;

		float m_timeSpan;
		uint32_t m_count = 0;
	};
}
