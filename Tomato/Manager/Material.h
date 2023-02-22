#pragma once
#include "glm/vec4.hpp"

namespace Tomato
{
	class Texture2D;

	enum class MaterialType : uint32_t
	{
		Albedo,
		Roughness,
		Metallic,
		Normal,
		Ao,
	};

	struct MaterialInfo
	{
		glm::vec4 albedo_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 roughness_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 metallic_color_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 emission_color_ = glm::vec4(0.0f, 0.0f, 0.0f, 10.0f);
		float     using_albedo_map_ = 1.0f;
		float     using_metallic_map_ = 1.0f;
		float     using_roughness_map_ = 1.0f;
		float     using_normal_map_ = 1.0f;
		float     using_ao_map_ = 1.0f;
		float     using_emission_map_ = 1.0f;

		//padding in vulkan
		float padding = 0.0f;
	};

	struct PBRMaterialTextures
	{
		std::shared_ptr<Texture2D> albedo_;
		std::shared_ptr<Texture2D> normal_;
		std::shared_ptr<Texture2D> metallic_;
		std::shared_ptr<Texture2D> roughness_;
		std::shared_ptr<Texture2D> ao_;
		std::shared_ptr<Texture2D> emission_;
	};

	class Material 
	{
	public:


	private:
		std::unordered_map<MaterialType, std::shared_ptr<Texture2D>> m_material_maps;
	};
}
