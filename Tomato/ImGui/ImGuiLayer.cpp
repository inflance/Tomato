#include "ImGuiLayer.hpp"

#include "Tomato/Renderer/Renderer.hpp"
#include "Tomato/Renderer/Vulkan/VulkanImGuiLayer.hpp"

namespace Tomato
{
	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return new VulkanImGuiLayer();
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}
}
