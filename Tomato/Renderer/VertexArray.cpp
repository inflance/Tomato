#include "VertexArray.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Renderer/Vulkan/VulkanVertexArray.h"

namespace Tomato
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanVertexArray>();
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
