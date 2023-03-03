#include "RenderPass.hpp"

#include "Renderer.hpp"
#include "RendererAPI.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

namespace Tomato
{
	Ref<RenderPass> RenderPass::Create(RenderPassInfo props)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanRenderPass>(props);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unknow");
	}
}
