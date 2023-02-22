#pragma once

#include "Tomato/Renderer/OrthographicCamera.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Events/MouseEvent.h"

namespace Tomato {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;
		OrthographicCameraBounds(float left, float right, float bottom, float top)
			:Left(left), Right(right), Top(top), Bottom(bottom)
		{}
		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthgraphicCameraControler
	{
	public:
		OrthgraphicCameraControler(float aspect, bool rotation = false);
		void Tick(float deltaTime);
		void OnEvent(Event& e);

		const OrthographicCameraBounds& GetBounds() const { return m_bounds; }
		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		void Resize(float width, float height);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool m_rotation;
		float m_aspect;
		float m_zoom = 1.0f;

		OrthographicCameraBounds m_bounds;
		OrthographicCamera m_camera;
		
		glm::vec3 m_camera_pos = {0.0f, 0.0f, 0.0f};
		float m_cameraMoveSpeed = 1.0f;

		float m_camera_rot = 0.0f;
		float m_cameraRotationSpeed = 90.0f;
	};
}