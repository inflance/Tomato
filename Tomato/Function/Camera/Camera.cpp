#include "Camera.hpp"

#include "glm/ext/matrix_clip_space.hpp"

namespace Tomato
{
	Camera::Camera(float fov, float aspect, float near, float far)
		: m_aspect_ratio(aspect), m_fov(fov), m_near(near), m_far(far)
	{
		SetCameraType(Type::Perspective);
	}


	Camera::Camera(float aspectRatio, float scale)
	{
	}


	Camera::Camera(float pitch, float yaw, const glm::vec3& position, float fov, float near, float far, float aspect)
	{
	}

	void Camera::UpdateProjectionMatrix()
	{
		if (m_camera_type == Type::Orthographic)
		{
			const float left = -m_zoom_level * m_aspect_ratio;
			const float right = -left;
			const float top = m_zoom_level;
			const float bottom = -top;
			m_project_matrix = glm::ortho(left, right, bottom, top, m_near, m_far);
		}
		else
		{
			m_project_matrix = glm::perspective(m_fov, m_aspect_ratio, m_near, m_far);
		}
	}
}
