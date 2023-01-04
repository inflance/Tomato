#pragma once

#include <memory>
#include <string>

#include "Texture.h"
#include "Tomato/Core/UUID.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace Tomato {

	using AssetID = UUID;

	enum class TextureType {
		Albedo = 0,//反射率
		Normal,//法线
		Metallic,//金属度
		Roughness,
		Ambient,
		Diffuse,
		Specular
	};
           
	struct MatirialTextureData {
		AssetID ID;
		TextureType Type;
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
		MatirialTextureData& GetDiffuseMap() { return GetMap(TextureType::Diffuse); }
		MatirialTextureData& GetSpecularMap() { return GetMap(TextureType::Specular); }
		MatirialTextureData& GetMap(TextureType type) { return m_MaterialMap[type]; }

		void SetAmbient(const glm::vec3& ambient){ m_Ambient = ambient; }
		void SetDiffuse(const glm::vec3& diffuse){ m_Diffuse = diffuse; }
		void SetSpecular(const glm::vec3& specular){ m_Specular = specular; }
		void SetShininess(float shininess){ m_Shininess = shininess; }
		void SetMap(TextureType type, MatirialTextureData& data) { m_MaterialMap[type] = data; }
		void SetDiffuseMap(MatirialTextureData& data) { SetMap(TextureType::Diffuse, data); };
		void SetSpecularMap(MatirialTextureData& data) { SetMap(TextureType::Specular, data); };
	private:
		glm::vec3 m_Ambient{ 0.1f };
		glm::vec3 m_Diffuse{ 1.0f };
		glm::vec3 m_Specular{ 1.0f };
		float	  m_Shininess = 32.0f;
		std::unordered_map<TextureType, MatirialTextureData> m_MaterialMap;
	};

}