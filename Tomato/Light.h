#pragma once

#include "glm/glm.hpp"

namespace Tomato {

	struct LightData
	{
		glm::vec3 Position;
	};

	class Light
	{
		virtual glm::vec3 Getintensity() = 0;
		virtual void SetIntensity(glm::vec3 intensity) = 0;
	};

	class DirectionLight : public Light
	{
		virtual glm::vec3 GetDirection() = 0;
		virtual void SetDirection(glm::vec3 direction) = 0;
	};

	class PointLight : public Light
	{

	};
}