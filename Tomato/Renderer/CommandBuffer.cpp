#include "CommandBuffer.hpp"

#include "Renderer.hpp"
#include "RendererAPI.hpp"

namespace Tomato
{
	Ref<CommandBuffer> CommandBuffer::Create(uint32_t count /*= 0*/, const std::string& debugName /*= ""*/)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}

	Ref<CommandBuffer> CommandBuffer::CreateFromSwapChain(const std::string& debugName /*= ""*/)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return nullptr;
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "δ֪API");
		return nullptr;
	}
}
