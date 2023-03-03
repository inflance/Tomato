#include "GraphicsContext.hpp"

#include "Renderer.hpp"
#include "Tomato/Core/Core.hpp"
#include "Vulkan/VulkanContext.hpp"

namespace Tomato
{
	std::shared_ptr<GraphicsContext> GraphicsContext::Create(Window* window)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanContext>(window);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
