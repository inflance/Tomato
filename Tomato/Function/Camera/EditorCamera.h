#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/MouseEvent.h"
#include "Tomato/Function/Controller/CameraControler.h"

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

	class EditorCamera : public CameraController<Camera>
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspect, float near, float far);

		void OnEvent(Event& e);

		void SetPosition(const glm::vec3& position) { m_position = position; }

		void SetViewportSize(float width, float height)
		{
			m_width = width;
			m_height = height;
			m_camera.SetAspectRatio(width / height);
			m_camera.UpdateProjectionMatrix();
		}

		[[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_view_mat; }
		[[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_camera.GetProjectionMatrix(); }

		[[nodiscard]] const glm::mat4& GetViewProjection() const
		{
			return GetViewMatrix() * m_camera.GetProjectionMatrix();
		}

		void SetCameraSpeed(std::pair<float, float> speed)
		{
			m_move_speed = speed.first;
			m_rot_speed = speed.second;
		}

		std::pair<float, float> GetCameraSpeed() { return {m_move_speed, m_rot_speed}; }
		std::tuple<glm::vec3, glm::vec3, glm::vec3> Get3() { return {m_position, m_target, m_up}; }

		void Set3(std::tuple<glm::vec3, glm::vec3, glm::vec3> three_vector)
		{
			m_position = std::get<0>(three_vector);
			m_target = std::get<1>(three_vector);
			m_up = std::get<2>(three_vector);
		}

		[[nodiscard]] glm::vec3 GetUp() const;
		[[nodiscard]] glm::vec3 GetRight() const;
		[[nodiscard]] glm::vec3 GetForward() const;
		[[nodiscard]] const glm::vec3& GetPosition() const { return m_position; }
		[[nodiscard]] glm::quat GetOrientation() const;

		[[nodiscard]] float GetPitch() const { return m_pitch; }
		[[nodiscard]] float GetYaw() const { return m_yaw; }

		void SetCameraType(CameraType type) { m_camera.SetCameraType(type); }
		[[nodiscard]] auto GetCameraType() const { return m_camera.GetCameraType(); }

	private:
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MoveCamera(CameraMove& movement, float delta_time);
		void CameraRotate(const glm::vec2& delta);
		void CameraZoom(float delta);


		[[nodiscard]] glm::vec3 CalculatePosition() const;

		[[nodiscard]] std::pair<float, float> PanSpeed() const;
		[[nodiscard]] float ZoomSpeed() const;

	private:
		float m_rot_speed = 30.0f;
		glm::mat4 m_view_mat{};
		glm::vec3 m_position{0, 0, 3};
		glm::vec3 m_target{0, 0, -1};
		glm::vec3 m_world_up{0, -1, 0};
		glm::vec3 m_up = m_world_up;
		glm::vec3 m_right{};
		glm::vec2 m_last_mouse_pos{};
		float m_zoom_level{0.0};
		float m_pitch{0.0f}, m_yaw = {-90};
		float m_move_speed = 8.0;
		float m_width = 1280, m_height = 720;
	};
}
