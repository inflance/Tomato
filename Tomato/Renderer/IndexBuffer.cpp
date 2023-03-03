#include "IndexBuffer.hpp"

#include "Renderer.hpp"
#include "RendererAPI.hpp"
#include "Tomato/Core/Macro.h"
#include "Vulkan/VulkanIndexBuffer.hpp"

namespace Tomato
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanIndexBuffer>(indices, count);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unknown ShaderDataType!");
		return nullptr;
	}
}
