#pragma once

#include "Tomato/Core/Layer.h"
#include "Tomato/Function/Camera/Camera.h"
#include "Tomato/Function/Camera/EditorCamera.h"
#include "Tomato/Function/Controller/CameraControler.h"
#include "Tomato/Renderer/IndexBuffer.h"
#include "Tomato/Renderer/Model.h"
#include "Tomato/Renderer/Texture.h"

namespace Tomato
{
	class UniformBuffer;
	class VertexBuffer;
	class Pipeline;
	class Event;

	class TestLayer : public Layer
	{

	public:
		TestLayer() :Layer("Test") {}
		void OnCreate() override;


		void OnDestroy() override;


		void Tick(float delta_time) override;


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
		CameraController<Ref<Camera>> m_controller;
		Ref<EditorCamera> m_editor_camera;
		Ref<Model> m_model = nullptr;
	};
}
