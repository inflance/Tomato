#include "RenderPass.h"

#include "Renderer.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanRenderPass.h"

namespace Tomato
{
	Ref<RenderPass> RenderPass::Create(const RenderPassProps& props)
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
