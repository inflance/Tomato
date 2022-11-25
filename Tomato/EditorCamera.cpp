#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Macro.h"

#include "Input.h"
#include "KeyCode.h"

namespace Tomato {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		//if()
		//m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.6f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float zoomLevel = m_ZoomLevel * 0.2f;
		zoomLevel = std::max(zoomLevel, 0.0f);
		float speed = zoomLevel * zoomLevel;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::Tick(float ts)
	{
		const glm::vec2& mousePos{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mousePos - m_lastMousePosition) * 0.003f;
		m_lastMousePosition = mousePos;
		float moveSpeed = ts * 10.0f;
		
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
				CameraMove(CameraMovement::Forward, moveSpeed);
			if (Input::IsKeyPressed(Key::S))
				CameraMove(CameraMovement::Back, moveSpeed);
			if (Input::IsKeyPressed(Key::A))
				CameraMove(CameraMovement::Left, moveSpeed);
			if (Input::IsKeyPressed(Key::D))
				CameraMove(CameraMovement::Right, moveSpeed);
			if (Input::IsKeyPressed(Key::E))
				CameraMove(CameraMovement::Up, moveSpeed);
			if (Input::IsKeyPressed(Key::Q))
				CameraMove(CameraMovement::Down, moveSpeed);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(&EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		CameraZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::CameraRotate(const glm::vec2& delta)
	{
		float yawSign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::CameraRotatoByTarget(const glm::vec2& delta)
	{
		float yawSign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::CameraZoom(float delta)
	{
		m_Position -= GetForward() * delta * ZoomSpeed();
		if (m_FOV < 1.0f)
		{
			m_FOV = 1.0f;
		}
		else if (m_FOV > 90.0f)
		{
			m_FOV = 90.0f;
		}
		UpdateView();
	}

	void EditorCamera::CameraMove(CameraMovement movement, float delta)
	{
		if (movement == CameraMovement::Forward)
			m_Position += GetForward() * delta;
		if (movement == CameraMovement::Back)
			m_Position -= GetForward() * delta;
		if (movement == CameraMovement::Left)
			m_Position -= GetRight() * delta;
		if (movement == CameraMovement::Right)
			m_Position += GetRight() * delta;
		if (movement == CameraMovement::Up)
			m_Position += GetUp() * delta;
		if (movement == CameraMovement::Down)
			m_Position -= GetUp() * delta;
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
		return m_Target - GetForward() * m_ZoomLevel;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

}