#include "Texture.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Platform/OpenGL/OpenGLTexture.h"

namespace Tomato {

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D> (path);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Tomato::TextureCube> TextureCube::Create(const std::vector<std::string>& paths)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTextureCube>(paths);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

	std::shared_ptr<Tomato::TextureCube> TextureCube::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTextureCube>(width, height);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

}