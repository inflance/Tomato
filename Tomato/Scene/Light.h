#pragma once

#include <glm/glm.hpp>

namespace Tomato {

	enum class LightType {
		DirectionLight = 0,
		PointLight,
		SpotLight
	};

	class Light {
	public:
		Light() = default;
		Light(LightType lightType, float intensity, const glm::vec4& color, const glm::vec3& direction)
			:m_light_type(lightType), m_intensity(intensity), m_color(color), m_direction(direction){}
		~Light() = default;

		void SetLightType(LightType lightType) { m_light_type = lightType; }
		void SetIntensity(float intensity) { m_intensity = intensity; }
		void SetColor(const glm::vec3& color) { m_color = color; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }

		LightType GetLightType() const { return m_light_type; }
		float GetIntensity() const { return m_intensity; }
		float GetConstant() const { return m_Constant; }
		float GetLinear() const { return m_Linear; }
		float GetQuadratic() const { return m_Quadratic; }
		glm::vec3& GetColor() { return m_color; }
		glm::vec3& GetDirection() { return m_direction; }

	private:
		LightType m_light_type = LightType::PointLight;
		float m_intensity = 1.0f;
		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;
		glm::vec3 m_color = glm::vec3(1.0f);
		//z axis
		glm::vec3 m_direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		
	};
}