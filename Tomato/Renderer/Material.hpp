#pragma once

#include <memory>
#include <string>

#include "Texture.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Tomato
{
	using AssetID = UUID;

	enum class PBRTextureType
	{
		Albedo = 0,
		//反射率
		Normal,
		//法线
		Metallic,
		//金属度
		Roughness,
		Ambient,
		Diffuse,
		Specular
	};

	struct MaterialTextureData
	{
		PBRTextureType Type;
		std::string Path;
		std::shared_ptr<Texture2D> Texture;
	};

	struct Material
	{
		alignas(16)glm::vec3 ambient_{ 0.1f };
		alignas(16)glm::vec3 diffuse_{1.0f};
		alignas(16)glm::vec3 specular_{1.0f};
		float shininess_ = 32.0f;
	};
}
