#include "FrameBuffer.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Tomato {

	std::shared_ptr<Tomato::FrameBuffer> FrameBuffer::Create(FrameBufferProps& props)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(props);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false,"未知֪API");
		return nullptr;
	}

}