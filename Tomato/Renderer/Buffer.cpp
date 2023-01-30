#include "Buffer.h"

#include "Renderer.h"
#include "Tomato/Core/Core.h"
#include "Tomato/Renderer/Vulkan/VulkanBuffer.h"

namespace Tomato
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanVertexBuffer>(vertices, size);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unknown ShaderDataType!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return std::make_shared<VulkanVertexBuffer>(size);
		case RendererAPI::API::DirectX12: return nullptr;
		}
		LOG_ASSERT(false, "Unknown ShaderDataType!");
		return nullptr;
	}

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
