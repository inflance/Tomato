#include "GraphicsContext.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Vulkan/VulkanContext.h"

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
