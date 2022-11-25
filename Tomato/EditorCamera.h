#pragma once

#include "Camera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Tomato {

	struct RotationAxis 
	{
		float Yaw = 0.0f, Pitch = 0.0f, Roll = 0.0f;

		operator glm::vec3()const {
			return glm::vec3(-Yaw, -Pitch, Roll);
		}
	};

	enum class CameraMovement {
		Forward = 0, Back,
		Left, Right, 
		Up, Down
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void Tick(float ts);
		void OnEvent(Event& e);

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_projection * m_ViewMatrix; }

		glm::vec3 GetUp() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetForward() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void CameraRotate(const glm::vec2& delta);
		void CameraRotatoByTarget(const glm::vec2& delta);
		void CameraZoom(float delta);
		void CameraMove(CameraMovement, float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_lastMousePosition = { 0.0f, 0.0f };

		float m_ZoomLevel = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}