#pragma once

#include "Renderer/OrthographicCamera.h"

#include "TimeSpan.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Tomato {

	class OrthgraphicCameraControler
	{
	public:
		OrthgraphicCameraControler(float aspect, bool rotation = false);
		void OnUpdate(TimeSpan ts);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_camera; };
		const OrthographicCamera& GetCamera() const { return m_camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool m_rotation;
		float m_aspect;
		float m_zoom = 1.0f;

		OrthographicCamera m_camera;

		glm::vec3 m_camera_pos = {0.0f, 0.0f, 0.0f};
		float m_cameraMoveSpeed = 1.0f;

		float m_camera_rot = 0.0f;
		float m_cameraRotationSpeed = 90.0f;
	};
}