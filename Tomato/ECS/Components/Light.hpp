#pragma once

#include <glm/glm.hpp>

namespace Tomato
{
	enum class LightType : uint32_t
	{
		DirectionLight = 0,
		PointLight,
		SpotLight
	};

	inline static std::unordered_map<LightType, std::string> light_type_to_str
	{
		{LightType::DirectionLight, "Direction Light"},
		{LightType::PointLight, "Point Light"},
		{LightType::SpotLight, "Spot Light"},
	};

	inline static std::unordered_map<std::string, LightType> str_to_light_type
	{
		{"Direction Light", LightType::DirectionLight},
		{"Point Light", LightType::PointLight},
		{"Spot Light", LightType::SpotLight,},
	};

	struct Light
	{
		LightType light_type_ = LightType::PointLight;
		alignas(16)glm::vec3 color_ = glm::vec3(1.0f);
		//z axis
		alignas(16)glm::vec3 position{};
		alignas(16)glm::vec3 direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
		float intensity_ = 1.0f;
		float constant_ = 1.0f;
		float linear_ = 0.09f;
		float quadratic_ = 0.032f;
		float cut_off = glm::cos(glm::radians(12.5f));
		float outer_cut_off = glm::cos(glm::radians(17.5f));
	};

	struct LightComponent
	{
		Light light_;

		LightComponent() = default;

		LightComponent(LightType lightType, float intensity, const glm::vec4& color, const glm::vec3& direction)
		{
		}
	};

}
