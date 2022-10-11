#include "Tomato/Core.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "Tomato/Platform/OpenGL/OpenGLVertexArray.h"

namespace Tomato {

	Tomato::Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "api");
		return nullptr;
	}

}