#include "ImGuiLayer.h"

#include "Tomato/Renderer/Renderer.h"
#include "Tomato/Renderer/Vulkan/VulkanImGuiLayer.h"

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
