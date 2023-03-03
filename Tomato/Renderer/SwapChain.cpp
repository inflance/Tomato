#include "SwapChain.hpp"

#include "Renderer.hpp"
#include "Vulkan/VulkanSwapChain.hpp"

namespace Tomato
{
	Ref<SwapChain> SwapChain::Create(Window* window, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanSwapChain>(window, width, height);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
