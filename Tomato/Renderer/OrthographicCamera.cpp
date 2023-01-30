#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Tomato
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_viewMat(1.0f), m_projectionMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		m_viewProjectionMat = m_projectionMat * m_viewMat;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_projectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMat = m_projectionMat * m_viewMat;
	}

	void OrthographicCamera::ReCalculateViewMatix()
	{
		glm::mat4 transform = translate(glm::mat4(1.0f), m_position)
			* rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMat = inverse(transform);
		m_viewProjectionMat = m_projectionMat * m_viewMat;
	}
}
