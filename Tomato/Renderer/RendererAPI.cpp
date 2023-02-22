#include "RendererAPI.h"

#include "Vulkan/VulkanRendererAPI.h"

namespace Tomato
{
	RendererAPI::API RendererAPI::s_api = API::Vulkan;

	Ref<RendererAPI> RendererAPI::Create()
	{
		switch (s_api)
		{
		case API::OpenGL: return nullptr;
		case API::Vulkan: return std::make_shared<VulkanRendererAPI>();
		case API::DirectX12: return nullptr;
		case API::None: break;
		}
		ASSERT(false);
		return nullptr;
	}
}
