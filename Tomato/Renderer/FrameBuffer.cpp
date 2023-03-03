#include "FrameBuffer.hpp"

#include "Renderer.hpp"
#include "Tomato/Core/Core.hpp"
#include "Vulkan/VulkanFrameBuffer.hpp"

namespace Tomato
{
	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferInfo& info)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanFrameBuffer>(info);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unkown API");
		return nullptr;
	}
}
