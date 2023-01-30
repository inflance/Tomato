#pragma once

#include <glm/glm.hpp>

namespace Tomato
{
	class Camera
	{
	public:
		Camera() = default;

		Camera(const glm::mat4& projection)
			: m_projection(projection)
		{
		}

		virtual ~Camera() = default;

		glm::mat4 GetProjection() const { return m_projection; }

	protected:
		glm::mat4 m_projection;
	};
}
