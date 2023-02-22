#include "GraphicsContext.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Platform/OpenGL/OpenGLContext.h"

namespace Tomato {

	std::shared_ptr<Tomato::GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLContext>(window);
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}