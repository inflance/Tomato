#pragma once

#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/Model.hpp"
#include "Tomato/Renderer/Texture.hpp"

namespace Tomato
{
	class AssetManager
	{
	public:
		static Ref<Texture2D> LoadTexture(std::string_view path);
		static Ref<Texture2D> LoadWhiteTexture();
		static const Ref<Model>& LoadModel(std::string_view path);
	};
}
