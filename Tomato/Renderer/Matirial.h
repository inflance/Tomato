#pragma once

#include <memory>
#include <string>

#include "Texture.h"
#include "Tomato/Core/UUID.h"


namespace Tomato {

	using AssetID = UUID;

	enum class TextureType {
		Albedo = 0,//������
		Normal,//����
		Metallic,//������
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

}