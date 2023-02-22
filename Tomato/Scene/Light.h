#pragma once

#include <glm/glm.hpp>

namespace Tomato
{
	enum class LightType
	{
		DirectionLight = 0,
		PointLight,
		SpotLight
	};

	struct Light
	{
		LightType light_type_ = LightType::PointLight;
		
		glm::vec3 color_ = glm::vec3(1.0f);
		//z axis
		glm::vec3 direction_ = glm::vec3(-0.2f, -1.0f, -0.3f);
		float intensity_ = 1.0f;
		float constant_ = 1.0f;
		float linear_ = 0.09f;
		float quadratic_ = 0.032f;
	};

}
