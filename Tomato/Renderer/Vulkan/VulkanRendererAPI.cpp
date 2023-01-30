#include "VulkanRendererAPI.h"

#include <vulkan/vulkan_core.h>

#include "VulkanSwapChain.h"
#include "Tomato/Core/Engine.h"

namespace Tomato
{
	
	VulkanRendererAPI::VulkanRendererAPI()
	{

	}

	void VulkanRendererAPI::Init()
	{
		//m_swap_chain = As<VulkanSwapChain>(TomatoEngine::Get().GetWindow().GetSwapChain());
	}

	void VulkanRendererAPI::SetViewPort(int x, int y, uint32_t width, uint32_t height)
	{
		//m_swap_chain->SetViewPort(x, y, width, height);
	}

	void VulkanRendererAPI::Clear()
	{

	}

	void VulkanRendererAPI::SetColor(const glm::vec4& color)
	{
		VkClearValue clearColor = { {color.r, color.g, color.b, color.a} };
	}

	void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count /*= 0*/)
	{
		
	}

	void VulkanRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vertex_array, uint32_t count /*= 0*/)
	{

	}

	void VulkanRendererAPI::Submit(const std::shared_ptr<Pipeline>& m_pipeline)
	{
		//auto swap_chain = As<VulkanSwapChain>(TomatoEngine::Get().GetWindow()->GetSwapChain());

		//swap_chain->Submit(m_pipeline);
	}
}

