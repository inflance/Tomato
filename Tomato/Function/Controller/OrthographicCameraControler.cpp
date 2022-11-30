#include "OrthographicCameraControler.h"

#include "Tomato/Core/Core.h"
#include "Tomato/Input/Input.h"
#include "Tomato/Input/Codes.h"

namespace Tomato {


	OrthgraphicCameraControler::OrthgraphicCameraControler(float aspect, bool rotation)
		:m_aspect(aspect), m_bounds(-m_aspect * m_zoom, m_aspect* m_zoom, -m_zoom, m_zoom), m_camera(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top), m_rotation(rotation)
	{

	}

	void OrthgraphicCameraControler::Tick(float deltaTime)
	{
		if (Tomato::Input::IsKeyPressed(Key::W)) {
			m_camera_pos.y += m_cameraMoveSpeed * deltaTime;
		}
		else if (Tomato::Input::IsKeyPressed(Key::S)) {
			m_camera_pos.y -= m_cameraMoveSpeed * deltaTime;
		}
		if (Tomato::Input::IsKeyPressed(Key::A)) {
			m_camera_pos.x -= m_cameraMoveSpeed * deltaTime;
		}
		else if (Tomato::Input::IsKeyPressed(Key::D)) {
			m_camera_pos.x += m_cameraMoveSpeed * deltaTime;
		}

		if (m_rotation) {
			if (Tomato::Input::IsKeyPressed(Key::Up)) {
				m_camera_rot += m_cameraRotationSpeed * deltaTime;
			}
			else if (Tomato::Input::IsKeyPressed(Key::Down)) {
				m_camera_rot -= m_cameraRotationSpeed * deltaTime;
			}
			m_camera.SetRotation(m_camera_rot);
		}
		m_camera.SetPosition(m_camera_pos);
		m_cameraMoveSpeed = m_zoom * 2.0f;
		
	}

	void OrthgraphicCameraControler::OnEvent(Event& e)
	{
		EventDispatcher dispather(e);
		dispather.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(&OrthgraphicCameraControler::OnMouseScrolled));
		//dispather.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(&OrthgraphicCameraControler::OnWindowResized));
	}



	bool OrthgraphicCameraControler::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoom -= e.GetYOffset() * 0.1f;
		m_zoom = std::max(m_zoom, 0.2f);
		m_bounds = { -m_aspect * m_zoom, m_aspect * m_zoom, -m_zoom, m_zoom };
		m_camera.SetProjection(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top);
		return false;
	}

	bool OrthgraphicCameraControler::OnWindowResized(WindowResizeEvent& e)
	{
		Resize((float)e.GetWidth() , (float)e.GetHeight());
		return false;
	}

	void OrthgraphicCameraControler::Resize(float width, float height)
	{
		m_aspect = width / height;
		m_bounds = { -m_aspect * m_zoom, m_aspect * m_zoom, -m_zoom, m_zoom };
		m_camera.SetProjection(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top);
	}

}