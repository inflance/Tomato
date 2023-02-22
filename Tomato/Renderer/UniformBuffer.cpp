#include "UniformBuffer.h"

#include "Renderer.h"
#include "Tomato/Renderer/Vulkan/VulkanUniformBuffer.h"

namespace Tomato
{
	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanUniformBuffer>(size, binding);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unknown API!");
		return nullptr;
	}
}
