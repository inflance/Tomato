#include "EditorCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Tomato/Core/Core.h"
#include "Tomato/Input/Input.h"

#undef near
#undef  far

namespace Tomato {

	EditorCamera::EditorCamera(float fov, float aspect_ratio, float near, float far)
		: m_fov(fov), m_aspect_ratio(aspect_ratio), m_near(near), m_far(far), Camera(glm::perspective(glm::radians(fov), aspect_ratio, near, far))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspect_ratio = m_viewport_width / m_viewport_height;
		m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
	}

	void EditorCamera::UpdateView()
	{
		const glm::quat orientation = GetOrientation();
		m_view_mat = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_view_mat = glm::inverse(m_view_mat);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_viewport_width / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewport_height / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.6f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float zoomLevel = m_zoom_level * 0.2f;
		zoomLevel = std::max(zoomLevel, 0.0f);
		float speed = zoomLevel * zoomLevel;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::Tick(float deltaTime)
	{
		const glm::vec2& mouse_pos{ Input::GetMouseX(), Input::GetMouseY() };
		const glm::vec2 delta = (mouse_pos - m_last_mouse_pos) * 0.003f;
		m_last_mouse_pos = mouse_pos;
		const float move_speed = deltaTime * 10.0f;
		
		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {
			//CameraRotate(delta);
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			//CameraRotatoByTarget(delta);
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
			CameraRotate(delta);
			if (Input::IsKeyPressed(Key::W))
				CameraMove(CameraMovement::Forward, move_speed);
			if (Input::IsKeyPressed(Key::S))
				CameraMove(CameraMovement::Back, move_speed);
			if (Input::IsKeyPressed(Key::A))
				CameraMove(CameraMovement::Left, move_speed);
			if (Input::IsKeyPressed(Key::D))
				CameraMove(CameraMovement::Right, move_speed);
			if (Input::IsKeyPressed(Key::E))
				CameraMove(CameraMovement::Up, move_speed);
			if (Input::IsKeyPressed(Key::Q))
				CameraMove(CameraMovement::Down, move_speed);
		}

		UpdateView();
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

	void EditorCamera::CameraRotate(const glm::vec2& delta)
	{
		const float yaw_sign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_yaw += yaw_sign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::CameraRotateByTarget(const glm::vec2& delta)
	{
		const float yaw_sign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_yaw += yaw_sign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::CameraZoom(float delta)
	{
		m_position -= GetForward() * delta * ZoomSpeed();
		if (m_fov < 1.0f){
			m_fov = 1.0f;
		}else if (m_fov > 90.0f){
			m_fov = 90.0f;
		}
		UpdateView();
	}

	void EditorCamera::CameraMove(CameraMovement movement, float delta)
	{
		if (movement == CameraMovement::Forward)
			m_position += GetForward() * delta;
		if (movement == CameraMovement::Back)
			m_position -= GetForward() * delta;
		if (movement == CameraMovement::Left)
			m_position -= GetRight() * delta;
		if (movement == CameraMovement::Right)
			m_position += GetRight() * delta;
		if (movement == CameraMovement::Up)
			m_position += GetUp() * delta;
		if (movement == CameraMovement::Down)
			m_position -= GetUp() * delta;
	}

	glm::vec3 EditorCamera::GetUp() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRight() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForward() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
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