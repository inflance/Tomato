#include "RendererCommand.h"

#include "Tomato/Core/Macro.h"
#include "Tomato/Renderer/Vulkan/VulkanRendererAPI.h"

namespace Tomato
{

	std::shared_ptr<RendererAPI> RendererCommand::Create()
	{
		switch(s_renderer_api->GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanRendererAPI>();
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
