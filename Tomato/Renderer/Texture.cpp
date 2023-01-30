#include "Texture.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Renderer/Vulkan/VulkanTexture.h"

namespace Tomato
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, TextureProperties properties)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTexture2D>(path, properties);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(TextureFormat formmat, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTexture2D>(formmat, width, height);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::string& path, TextureProperties properties)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTextureCube>(path, properties);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(TextureFormat formmat, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTextureCube>(formmat, width, height);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}
}
