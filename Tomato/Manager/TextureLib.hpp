#pragma once

#include <unordered_map>

#include "Tomato/Core/Macro.h"
#include "Tomato/Core/Singleton.h"
#include "Tomato/Renderer/Texture.hpp"

namespace Tomato
{
	class TextureLib : public Singleton<TextureLib>
	{
	public:
		[[nodiscard]] bool Exist(const std::string& path) const;
		bool Add(const std::string& path, Ref<Texture2D> tex);
		Ref<Texture> GetTexture(const std::string& path);

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_tex_lib;
	};
}
