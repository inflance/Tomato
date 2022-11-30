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
		void SetColor(const glm::vec4& color) { m_color = color; }
		void SetDirection(const glm::vec3& direction) { m_direction = direction; }

		LightType GetLightType() const { return m_light_type; }
		float GetIntensity() const { return m_intensity; }
		const glm::vec4& GetColor() const { return m_color; }
		const glm::vec3& GetDirection() const { return m_direction; }

	private:
		LightType m_light_type = LightType::DirectionLight;
		float m_intensity = 1.0f;

		glm::vec4 m_color = glm::vec4(1.0f);
		//z axis
		glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	};
}