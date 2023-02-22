#pragma once

#include <glm/glm.hpp>

namespace Tomato {

	class OrthographicCamera 
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(glm::vec3 position) { m_position = position; ReCalculateViewMatix();}
		const glm::vec3& GetPosition() const { return m_position; }

		void SetRotation(float rotation) { m_rotation = rotation;  ReCalculateViewMatix();}
		float GetRotation() const { return m_rotation; }
		
		const glm::mat4 GetViewMat() const { return m_viewMat; }
		const glm::mat4 GetProjectionMat() const { return m_projectionMat; }
		const glm::mat4 GetViewProjectionMat() const { return m_viewProjectionMat; }

	private:
		void ReCalculateViewMatix();
	private:
		glm::vec3 m_position = {0.0f, 0.0f, 0.0f};

		glm::mat4 m_viewMat;
		glm::mat4 m_projectionMat;
		glm::mat4 m_viewProjectionMat;

		float m_rotation = 0.0f;
	};
}
