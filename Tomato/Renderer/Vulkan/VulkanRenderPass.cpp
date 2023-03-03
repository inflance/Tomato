#include "Tomato/pch.h"
#include "VulkanRenderPass.hpp"

#include "VulkanContext.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanTexture.hpp"

namespace Tomato
{
	VulkanRenderPass::VulkanRenderPass(RenderPassInfo props)
		: m_props(std::move(props))
	{
		Init();
		InitRenderPass();
	}
	void VulkanRenderPass::Init()
	{
		if (!m_props.attachments_.empty()) m_props.attachments_.clear();

		if (!m_props.format_.empty())
		{
			m_props.attachments_.reserve(m_props.format_.size());
			for (int i = 0; i < m_props.attachments_.size(); i++)
			{
				TextureInfo info;
				info.format_ = m_props.format_[i];
				if (info.format_ <= TextureFormat::Color)
				{
					info.usage_ = TextureUsage::Color;
					info.type_ = TextureType::Color;
					info.multi_sampler_rate = m_props.sample_count;
					info.extend_.width_ = m_props.width_;
					info.extend_.height_ = m_props.height_;
				}
				else
				{
					info.usage_ = TextureUsage::Depth;
					info.type_ = TextureType::Depth;
					info.multi_sampler_rate = m_props.sample_count;
					info.extend_.width_ = m_props.width_;
					info.extend_.height_ = m_props.height_;
				}
				m_props.attachments_[i] = Texture2D::Create(info);
			}
		}
		else
		{
			m_props.attachments_.reserve(2);
			TextureInfo info;
			info.format_ = TextureFormat::Color;
			info.usage_ = TextureUsage::Color;
			info.type_ = TextureType::Color;
			info.multi_sampler_rate = m_props.sample_count;
			info.extend_.width_ = m_props.width_;
			info.extend_.height_ = m_props.height_;
			m_props.attachments_.emplace_back(Texture2D::Create(info));
			info.format_ = TextureFormat::Depth;
			info.usage_ = TextureUsage::Depth;
			info.type_ = TextureType::Depth;
			m_props.attachments_.emplace_back(Texture2D::Create(info));
		}

	}

	void VulkanRenderPass::InitRenderPass()
	{
		const auto& device = VulkanContext::Get().device;
		std::vector<vk::AttachmentDescription> color_attachment;
		std::vector<vk::AttachmentDescription> resorve_attachment;
		std::vector<vk::AttachmentDescription> depth_attachment;

		for (auto& attach : m_props.attachments_)
		{
			if (attach->GetType() == TextureType::Color)
			{
				vk::AttachmentDescription colorAttach{};
				colorAttach.setFormat(As<VulkanTexture2D>(attach)->format)
					.setSamples(As<VulkanTexture2D>(attach)->sample_count)
					.setLoadOp(vk::AttachmentLoadOp::eClear)
					.setStoreOp(vk::AttachmentStoreOp::eStore)
					.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
					.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
					.setInitialLayout(vk::ImageLayout::eUndefined)
					.setFinalLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
				color_attachment.emplace_back(colorAttach);
				if(m_props.sample_count.has_value())
				{
					vk::AttachmentDescription resorveAttach{};
					resorveAttach.setFormat(As<VulkanTexture2D>(attach)->format)
						.setSamples(vk::SampleCountFlagBits::e1)
						.setLoadOp(vk::AttachmentLoadOp::eDontCare)
						.setStoreOp(vk::AttachmentStoreOp::eStore)
						.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
						.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
						.setInitialLayout(vk::ImageLayout::eUndefined)
						.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
					resorve_attachment.emplace_back(resorveAttach);
				}
			}
			else if (attach->GetType() == TextureType::Depth)
			{
				vk::AttachmentDescription depthAttach{};
				depthAttach.setFormat(As<VulkanTexture2D>(attach)->format)
					.setSamples(As<VulkanTexture2D>(attach)->sample_count)
					.setLoadOp(vk::AttachmentLoadOp::eClear)
					.setStoreOp(vk::AttachmentStoreOp::eDontCare)
					.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
					.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
					.setInitialLayout(vk::ImageLayout::eUndefined)
					.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
				depth_attachment.emplace_back(depthAttach);
			}
		}
		std::vector<vk::AttachmentDescription> attachments;
		attachments.insert(attachments.end(), color_attachment.cbegin(), color_attachment.cend());
		attachments.insert(attachments.end(), resorve_attachment.cbegin(), resorve_attachment.cend());
		attachments.insert(attachments.end(), depth_attachment.cbegin(), depth_attachment.cend());

		vk::RenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.setAttachments(attachments);

		//attachment is the index of the AttachmentDescription array;
		vk::AttachmentReference colorReference{};
		colorReference.setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		vk::AttachmentReference resolveReference{};
		if (m_props.sample_count.has_value())
		{
			resolveReference.setAttachment(color_attachment.size()).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

		}
		vk::AttachmentReference depthReference{};
		depthReference.setAttachment(color_attachment.size()+resorve_attachment.size()).
			setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		std::array<vk::SubpassDependency, 2> dependencis;
		dependencis[0].setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
			.setSrcStageMask(vk::PipelineStageFlagBits::eFragmentShader)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDependencyFlags(vk::DependencyFlagBits::eByRegion);

		dependencis[1].setSrcSubpass(0)
			.setDstSubpass(VK_SUBPASS_EXTERNAL)
			.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eShaderRead)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eFragmentShader).setDependencyFlags(
				vk::DependencyFlagBits::eByRegion);

		vk::SubpassDescription subpass{};
		subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(colorReference)
			.setColorAttachmentCount(color_attachment.size())
			.setPDepthStencilAttachment(&depthReference);
		if(m_props.sample_count.has_value())
		{
			subpass.setPResolveAttachments(&resolveReference);
		}
		renderPassCreateInfo
			.setDependencies(dependencis)
			.setDependencyCount(dependencis.size())
			.setSubpasses(subpass)
			.setSubpassCount(1);
		renderPass = vk::raii::RenderPass(device, renderPassCreateInfo);
	}

	void VulkanRenderPass::OnResize(uint32_t width, uint32_t height)
	{
		m_props.width_ = width;
		m_props.height_ = height;
		Init();
		InitRenderPass();
	}
}
