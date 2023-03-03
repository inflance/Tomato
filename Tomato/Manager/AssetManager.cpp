#include "AssetManager.hpp"

#include "stb_image.h"
#include "TextureLib.hpp"
#include "Tomato/Renderer/Renderer.hpp"

namespace Tomato
{
	Ref<Texture2D> AssetManager::LoadTexture(std::string_view path)
	{
		int w, h, channel;
		stbi_uc* pixels = stbi_load(path.data(), &w, &h, &channel, STBI_rgb_alpha);

		if (!pixels)
		{
			LOG_ERROR("Failed to load file :{}", path);
		}

		TextureInfo info{};
		info.mip_levels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(w, h)))) + 1;
		info.extend_.width_ = w;
		info.extend_.height_ = h;
		info.stage_ = true;
		info.gen_mips_ = false;
		TextureLib::Get().Add(path.data(), Texture2D::Create(pixels, info));
		stbi_image_free(pixels);
		return As<Texture2D>(TextureLib::Get().GetTexture(path.data()));
	}

	Ref<Texture2D> AssetManager::LoadWhiteTexture()
	{
		uint8_t pixels[] = { 0xFF, 0xFF, 0xFF, 0xFF };
		TextureInfo info{};
		info.mip_levels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(1, 1)))) + 1;
		info.extend_.width_ = 1;
		info.extend_.height_ = 1;
		TextureLib::Get().Add("white", Texture2D::Create(pixels, info));
		return As<Texture2D>(TextureLib::Get().GetTexture("white"));
	}

	const Ref<Model>& AssetManager::LoadModel(std::string_view path)
	{
		return nullptr;
	}
}
