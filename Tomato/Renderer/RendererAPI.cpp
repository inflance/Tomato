#include "RendererAPI.hpp"


#include "GraphicsContext.hpp"
#include "IndexBuffer.hpp"
#include "Model.hpp"

#include "Vulkan/VulkanRendererAPI.hpp"

namespace Tomato
{
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
