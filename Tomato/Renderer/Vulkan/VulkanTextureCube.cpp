#include "VulkanTextureCube.hpp"

#include "VulkanCommandExecutor.hpp"
#include "VulkanContext.hpp"

namespace Tomato
{
	VulkanTextureCube::VulkanTextureCube()
	{
	}

	void VulkanTextureCube::TransitionImageLayout(const vk::raii::CommandBuffer& cmdBuf, vk::ImageLayout lodLay, vk::ImageLayout newLay, vk::AccessFlagBits srcMask, vk::AccessFlagBits dstMask, vk::PipelineStageFlagBits srcPipMask,
	                          vk::PipelineStageFlagBits dstPipMask)
	{
		vk::ImageMemoryBarrier barrier;
		vk::ImageSubresourceRange range;
		range
			.setAspectMask(vk::ImageAspectFlagBits::eColor)
			.setBaseMipLevel(0)
			.setLevelCount(1)
			.setBaseArrayLayer(0)
			.setLayerCount(6);
		barrier
			.setImage(*image)
			.setOldLayout(lodLay)
			.setNewLayout(newLay)
			.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setSrcAccessMask(srcMask)
			.setDstAccessMask(dstMask)
			.setSubresourceRange(range);
		cmdBuf.pipelineBarrier(
			srcPipMask,
			dstPipMask, {}, {}, nullptr, barrier);
	}

	void VulkanTextureCube::CopyDataToImage(vk::raii::CommandBuffer& cmd, const vk::raii::Buffer& buffer,
		uint32_t width, uint32_t height)
	{
	}

	VulkanTextureCube::VulkanTextureCube(Buffer buffer, TextureInfo info)
	{
		format = vk::Format::eR8G8B8A8Unorm;
		uint32_t size = m_info.extend_.width_ * m_info.extend_.height_ * 4;
		auto width = m_info.extend_.width_;
		auto height = m_info.extend_.width_;
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;

		CreateImage(m_info.extend_.width_, m_info.extend_.height_);
		AllocateImageMemory();
		// bind memory
		image.bindMemory(*memory, 0);
		//stage buffer
		vkBuffer stage_buffer{ nullptr };
		if (m_need_stage)
		{
			stage_buffer = vkBuffer(physicalDevice, device, size, vk::BufferUsageFlagBits::eTransferSrc);
		}

		auto pixel = m_need_stage
			? stage_buffer.memory.mapMemory(0, size, vk::MemoryMapFlags())
			: memory.mapMemory(0, size, vk::MemoryMapFlags());
		memcpy(pixel, buffer.data_, size);
		m_need_stage ? stage_buffer.memory.unmapMemory() : memory.unmapMemory();


		if (m_need_stage)
		{
			CommandExecutor::Get().ImmediateExecute(
				VulkanContext::Get().graphicsQueue,
				[&](vk::raii::CommandBuffer& cmd)
				{
					TransitionImageLayout(
						cmd, vk::ImageLayout::eUndefined,
						vk::ImageLayout::eTransferDstOptimal, {},
						vk::AccessFlagBits::eTransferWrite,
						vk::PipelineStageFlagBits::eTransfer,
						vk::PipelineStageFlagBits::eTransfer);
			CopyDataToImage(cmd, stage_buffer.buffer, width, height);

			TransitionImageLayout(
				cmd, vk::ImageLayout::eTransferDstOptimal,
				vk::ImageLayout::eTransferSrcOptimal,
				vk::AccessFlagBits::eTransferWrite,
				vk::AccessFlagBits::eTransferRead,
				vk::PipelineStageFlagBits::eTransfer,
				vk::PipelineStageFlagBits::eTransfer);
				});
		}
		else
		{
			CommandExecutor::Get().ImmediateExecute(
				VulkanContext::Get().graphicsQueue,
				[&](const vk::raii::CommandBuffer& cmd)
				{
					TransitionImageLayout(cmd, vk::ImageLayout::ePreinitialized,
					vk::ImageLayout::eShaderReadOnlyOptimal,
					vk::AccessFlagBits::eHostWrite,
					vk::AccessFlagBits::eShaderRead, vk::PipelineStageFlagBits::eHost,
					vk::PipelineStageFlagBits::eFragmentShader);
				});
		}
		CreateImageView();
		CreateSampler();
		layout = vk::ImageLayout::eShaderReadOnlyOptimal;
		UpdateDescriptor();
	}
	VulkanTextureCube::~VulkanTextureCube()
	{
	}

	void VulkanTextureCube::CreateImage(uint32_t width, uint32_t height)
	{
		vk::ImageCreateInfo create_info;

		create_info
			.setImageType(vk::ImageType::e2D)
			.setArrayLayers(1)
			.setMipLevels(m_info.mip_levels_)
			.setExtent({ width, height, 1 })
			.setFormat(format)
			.setTiling(m_need_stage ? vk::ImageTiling::eOptimal : vk::ImageTiling::eLinear)
			.setInitialLayout((m_need_stage ? vk::ImageLayout::eUndefined : vk::ImageLayout::ePreinitialized))
			.setUsage(vk::ImageUsageFlagBits::eSampled |
				(m_need_stage
					? vk::ImageUsageFlagBits::eTransferDst
					: vk::ImageUsageFlagBits()) |
				vk::ImageUsageFlagBits::eTransferSrc)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setSharingMode(vk::SharingMode::eExclusive);


		image = VulkanContext::Get().device.createImage(create_info);
	}

	void VulkanTextureCube::AllocateImageMemory()
	{
	}

	void VulkanTextureCube::CreateImageView()
	{
	}

	void VulkanTextureCube::CreateSampler()
	{
	}

	void VulkanTextureCube::UpdateDescriptor()
	{
	}
	void VulkanTextureCube::SetData(void* data, uint32_t size)
	{
	}
	void VulkanTextureCube::BindUnit(uint32_t slot) const
	{
	}

	void VulkanTextureCube::Bind() const
	{
	}

	bool VulkanTextureCube::operator==(const Texture& other) const
	{
		return true;
	}
}
