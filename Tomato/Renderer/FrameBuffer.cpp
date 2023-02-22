#include "FrameBuffer.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"

namespace Tomato
{
	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferProps& props)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unkown API");
		return nullptr;
	}
}
