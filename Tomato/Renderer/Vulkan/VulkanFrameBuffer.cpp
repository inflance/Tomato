#include "VulkanFrameBuffer.hpp"

#include "VulkanContext.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanTexture.hpp"

namespace Tomato
{
	VulkanFrameBuffer::VulkanFrameBuffer(const FrameBufferInfo& info)
		: m_info(info)
	{
		Init();
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
	}
	void VulkanFrameBuffer::Init()
	{
		auto& pd = VulkanContext::Get().physicalDevice;
		auto limit = pd;
		ASSERT(m_info.height_);
		std::vector<vk::ImageView> attachments;


		for (auto& tex : m_info.render_pass->GetProps().attachments_)
		{
			if(tex->GetType() == TextureType::Color)
			{
				attachments.push_back(*As<VulkanTexture2D>(tex)->imageView);
				if(As<VulkanRenderPass>(m_info.render_pass)->GetProps().sample_count)
				{
					attachments.push_back(*VulkanContext::Get().swapChain->imageViews[0]);
				}
			}else
			{
				attachments.push_back(*As<VulkanTexture2D>(tex)->imageView);
			}
		}

		vk::FramebufferCreateInfo create_info;
		create_info
			.setAttachments(attachments)
			.setAttachmentCount(attachments.size())
			.setFlags({})
			.setHeight(m_info.height_)
			.setWidth(m_info.width_)
			.setLayers(m_info.layer_)
			.setRenderPass(*As<VulkanRenderPass>(m_info.render_pass)->GetHandle());

		m_framebuffer = vk::raii::Framebuffer(VulkanContext::Get().device, create_info);
	}

	void VulkanFrameBuffer::OnResize(uint32_t width, uint32_t height)
	{
		const auto& device = VulkanContext::Get().device;
		device.waitIdle();
		m_info.width_ = width;
		m_info.height_ = height;
		m_info.render_pass->OnResize(width, height);
		Init();
		device.waitIdle();
	}
}
