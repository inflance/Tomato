#include "Texture.hpp"

#include "Renderer.hpp"
#include "Tomato/Core/Core.hpp"
#include "Tomato/Renderer/Vulkan/VulkanTexture.hpp"

namespace Tomato
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path, TextureInfo info)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTexture2D>(path);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(void* data,TextureInfo info)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTexture2D>(data, info);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(TextureInfo info)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanTexture2D>(info);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::string& path, TextureInfo info)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

}
