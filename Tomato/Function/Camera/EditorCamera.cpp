#include "EditorCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Tomato/Core/Core.h"
#include "Tomato/Input/Input.h"

namespace Tomato
{
	EditorCamera::EditorCamera(float fov, float aspect, float near, float far)
		: CameraController<Camera>({fov, aspect, near, far})
	{
		auto EditorCameraTick = [&](Camera& camera, float delta_time)
		{
			const glm::vec2& mouse_pos{Input::GetMouseX(), Input::GetMouseY()};
			const glm::vec2 delta = (mouse_pos - m_last_mouse_pos) * 0.003f;
			m_last_mouse_pos = mouse_pos;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			{
				//CameraRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{
				//CameraRotatoByTarget(delta);
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			{
				CameraRotate(delta);
				CameraMove movement{};
				LOG_INFO("{}", CameraMove::Forward);
				LOG_INFO("{}", CameraMove::Back);
				LOG_INFO("{}", CameraMove::Left);
				if (Input::IsKeyPressed(Key::W))
				{
					movement |= CameraMove::Forward;
					LOG_INFO("{}", movement);
				}
				if (Input::IsKeyPressed(Key::S))
					movement |= CameraMove::Back;
				if (Input::IsKeyPressed(Key::A))
					movement |= CameraMove::Left;
				if (Input::IsKeyPressed(Key::D))
					movement |= CameraMove::Right;
				if (Input::IsKeyPressed(Key::E))
					movement |= CameraMove::Up;
				if (Input::IsKeyPressed(Key::Q))
				{
					movement |= CameraMove::Down;

				}
				MoveCamera(movement, delta_time);
			}
			UpdateView();
		};
		SetCameraTickFnc(EditorCameraTick);

		UpdateView();
	}


	void EditorCamera::UpdateView()
	{
		
		glm::vec3 front{};
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_target = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_target, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_target));
		m_view_mat = glm::lookAt(m_position, m_position + m_target, m_up);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_width / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_height / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
	}


	float EditorCamera::ZoomSpeed() const
	{
		float zoomLevel = m_zoom_level * 0.2f;
		zoomLevel = std::max(zoomLevel, 0.0f);
		float speed = zoomLevel * zoomLevel;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		const float delta = e.GetYOffset() * 0.1f;
		CameraZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MoveCamera(CameraMove& movement, float delta_time)
	{
		if (static_cast<bool>(movement & CameraMove::Forward))
		{
			m_position += GetForward() * delta_time * m_move_speed;
		}
		if (static_cast<bool>(movement & CameraMove::Back))
		{
			m_position -= GetForward() * delta_time * m_move_speed;
		}
		if (static_cast<bool>(movement & CameraMove::Left))
		{
			m_position -= GetRight() * delta_time * m_move_speed;
		}
		if (static_cast<bool>(movement & CameraMove::Right))
		{
			m_position += GetRight() * delta_time * m_move_speed;
		}
		if (static_cast<bool>(movement & CameraMove::Up))
		{
			m_position -= GetUp() * delta_time * m_move_speed; // vulkan 0 ,0 is left top 
		}
		if (static_cast<bool>(movement & CameraMove::Down))
		{
			m_position += GetUp() * delta_time * m_move_speed;
		}
	}

	void EditorCamera::CameraRotate(const glm::vec2& delta)
	{
		const float yaw_sign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_yaw += yaw_sign * delta.x * m_rot_speed;
		m_pitch -= delta.y * m_rot_speed;

	}

	void EditorCamera::CameraZoom(float delta)
	{
		m_position -= GetForward() * delta * ZoomSpeed();
		if (m_camera.GetFov() < 1.0f)
		{
			m_camera.SetFov(1.0f);
		}
		else if (m_camera.GetFov() > 90.0f)
		{
			m_camera.SetFov(90.0f);
		}
		UpdateView();
	}

	glm::vec3 EditorCamera::GetUp() const
	{
		return m_up;
	}

	glm::vec3 EditorCamera::GetRight() const
	{
		return m_right;
	}

	glm::vec3 EditorCamera::GetForward() const
	{
		return m_target;
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_target - GetForward() * m_zoom_level;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}
}
