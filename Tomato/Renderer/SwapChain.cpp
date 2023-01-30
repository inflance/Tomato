#include "SwapChain.h"

#include "Renderer.h"
#include "Vulkan/VulkanSwapChain.h"

namespace Tomato
{
	Ref<SwapChain> SwapChain::Create(uint32_t width, uint32_t height)
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
