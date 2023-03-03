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

	struct MatirialTextureData
	{
		PBRTextureType Type;
		std::string Path;
		std::shared_ptr<Texture2D> Texture;
	};

	class Matirial
	{
	public:
		[[nodiscard]] const glm::vec3& GetAmbient() const { return m_Ambient; }
		[[nodiscard]] const glm::vec3& GetDiffuse() const { return m_Diffuse; }
		[[nodiscard]] const glm::vec3& GetSpecular() const { return m_Specular; }
		[[nodiscard]] float GetShininess() const { return m_Shininess; }
		MatirialTextureData& GetDiffuseMap() { return GetMap(PBRTextureType::Diffuse); }
		MatirialTextureData& GetSpecularMap() { return GetMap(PBRTextureType::Specular); }
		MatirialTextureData& GetMap(PBRTextureType type) { return m_MaterialMap[type]; }

		void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
		void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
		void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
		void SetShininess(float shininess) { m_Shininess = shininess; }
		void SetMap(PBRTextureType type, MatirialTextureData& data) { m_MaterialMap[type] = data; }
		void SetDiffuseMap(MatirialTextureData& data) { SetMap(PBRTextureType::Diffuse, data); };
		void SetSpecularMap(MatirialTextureData& data) { SetMap(PBRTextureType::Specular, data); };

	private:
		glm::vec3 m_Ambient{0.1f};
		glm::vec3 m_Diffuse{1.0f};
		glm::vec3 m_Specular{1.0f};
		float m_Shininess = 32.0f;
		std::unordered_map<PBRTextureType, MatirialTextureData> m_MaterialMap;
	};
}
