#pragma once

#include "Tomato/Core/Layer.h"

namespace Tomato
{
	class SubTexture2D;
	class Level;
	class ViewPortPanel;
	class Model;
	class EditorCamera;
	class Camera;
	class Texture2D;
	class IndexBuffer;
	class LevelPanel;
	class RenderPass;
	class FrameBuffer;
	class UniformBuffer;
	class VertexBuffer;
	class Pipeline;
	class Event;

	enum class GzmoOp
	{
		None = -1,
		Translate,
		Rotate,
		Scale
	};

	enum class EditorType
	{
		EditorMode,
		RuntimeMode
	};

	class TestLayer : public Layer
	{

	public:
		TestLayer() :Layer("Test") {}

		void OnCreate() override;

		void OnDestroy() override;

		void Tick(float delta_time) override;

		void CreateNewLevel();

		void OpenLevel();

		void SaveLevelAs() const;

		void OnImGuiRenderer() override;

		void OnEvent(Event& event) override;

		bool OnKeyPressed(KeyPressedEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Ref<Pipeline> pipeline = nullptr;
		Ref<IndexBuffer> index_buffer = nullptr;
		Ref<VertexBuffer> vertex_buffer = nullptr;
		Ref<VertexBuffer> vertex_buffer1 = nullptr;
		Ref<UniformBuffer> uniformBuffer = nullptr;
		Ref<Texture2D> texture = nullptr;
		Ref<Camera> m_camera = nullptr;
		Ref<EditorCamera> m_editor_camera;
		Ref<RenderPass> renderPass;
		Ref<Model> m_model = nullptr;
		Ref<FrameBuffer> m_frame_buffer;
		Ref<ViewPortPanel> m_view_port_panel;
		Ref<LevelPanel> m_level_panel;
		Ref<Level> m_level;
		Ref<SubTexture2D> m_sub_tex;
		bool is_resize = false;
		EditorType m_editor_mode;
		GzmoOp m_gzmo_op = GzmoOp::Translate;
	};
}
