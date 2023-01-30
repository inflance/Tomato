#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/MouseEvent.h"

namespace Tomato
{
	struct RotationAxis
	{
		float Yaw = 0.0f, Pitch = 0.0f, Roll = 0.0f;

		operator glm::vec3() const
		{
			return {-Yaw, -Pitch, Roll};
		}
	};

	enum class CameraMovement
	{
		Forward = 0,
		Back,
		Left,
		Right,
		Up,
		Down
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void Tick(float deltaTime);
		void OnEvent(Event& e);

		[[nodiscard]] float GetZoomLevel() const { return m_zoom_level; }
		[[nodiscard]] void SetZoomLevel(float zoomLevel) { m_zoom_level = zoomLevel; }
		void SetPosition(const glm::vec3& position) { m_position = position; }

		void SetViewportSize(float width, float height)
		{
			m_viewport_width = width;
			m_viewport_height = height;
			UpdateProjection();
		}

		[[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_view_mat; }
		[[nodiscard]] glm::mat4 GetViewProjection() const { return m_projection * m_view_mat; }

		[[nodiscard]] glm::vec3 GetUp() const;
		[[nodiscard]] glm::vec3 GetRight() const;
		[[nodiscard]] glm::vec3 GetForward() const;
		[[nodiscard]] const glm::vec3& GetPosition() const { return m_position; }
		[[nodiscard]] glm::quat GetOrientation() const;

		[[nodiscard]] float GetPitch() const { return m_pitch; }
		[[nodiscard]] float GetYaw() const { return m_yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void CameraRotate(const glm::vec2& delta);
		void CameraRotateByTarget(const glm::vec2& delta);
		void CameraZoom(float delta);
		void CameraMove(CameraMovement, float delta);

		[[nodiscard]] glm::vec3 CalculatePosition() const;

		[[nodiscard]] std::pair<float, float> PanSpeed() const;
		[[nodiscard]] float RotationSpeed() const;
		[[nodiscard]] float ZoomSpeed() const;

	private:
		float m_fov = 45.0f, m_aspect_ratio = 1.778f, m_near = 0.1f, m_far = 1000.0f;

		glm::mat4 m_view_mat;
		glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_target = {0.0f, 0.0f, 0.0f};
		glm::vec2 m_last_mouse_pos = {0.0f, 0.0f};

		float m_zoom_level = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;

		float m_viewport_width = 1280, m_viewport_height = 720;
	};
}
