#include "TextureLib.h"

namespace Tomato
{
	bool TextureLib::Exist(const std::string& path) const
	{
		return m_tex_lib.contains(path);
	}

	bool TextureLib::Add(const std::string& path, Ref<Texture2D> tex)
	{
		if(Exist(path))
		{
			LOG_ERROR("Texture {} is existd!", path);
			return false;
		}
		m_tex_lib[path] = tex;
		return true;
	}

	Ref<Texture> TextureLib::GetTexture(const std::string& path)
	{
		if (!Exist(path))
		{
			LOG_ERROR("Texture {} is not existd!", path);
			return nullptr;
		}
		return m_tex_lib.at(path);
	}
}
