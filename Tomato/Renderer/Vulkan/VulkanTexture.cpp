#include "VulkanTexture.h"
#include "stb_image.h"
#include "VulkanCommandExecutor.h"
#include "VulkanContext.h"
#include "Tomato/Core/Macro.h"

namespace Tomato
{
	namespace Utils
	{
		static auto ToVulkanFilter(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::Linear:
				return vk::Filter::eLinear;
			case TextureFilter::Nearest:
				return vk::Filter::eNearest;
			case TextureFilter::Cubic:
				return vk::Filter::eCubicEXT;
			}
			LOG_ERROR("Unknown filter!");
			ASSERT(false);
		}
	}

	VulkanTexture2D::VulkanTexture2D(const std::string& path)
	{
		LOG_INFO("VulkanTexture2D");
		int w, h, channel;
		stbi_uc* pixels = stbi_load(path.data(), &w, &h, &channel, STBI_rgb_alpha);

		if (!pixels)
		{
			LOG_ERROR("Failed to load file :{}", path);
			ASSERT(false);
		}
		m_info.extend_.width_ = w;
		m_info.extend_.height_ = h;

		m_info.mip_levels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(w, h)))) + 1;

		Init(pixels);

		stbi_image_free(pixels);
	}

	VulkanTexture2D::VulkanTexture2D()
	{
	}

	VulkanTexture2D::VulkanTexture2D(void* data, TextureInfo info)
		: m_info(info)
	{
		Init(data);
	}


	void VulkanTexture2D::Init(void* data)
	{
		uint32_t size = m_info.extend_.width_ * m_info.extend_.height_ * 4;
		auto width = m_info.extend_.width_;
		auto height = m_info.extend_.width_;
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;

		CreateImage(m_info.extend_.width_, m_info.extend_.height_);
		AllocateImageMemory();
		// bind memory
		image.bindMemory(*memory, 0);

		Buffer stage_buffer{nullptr};
		if (m_need_stage)
		{
			stage_buffer = Buffer(physicalDevice, device, size, vk::BufferUsageFlagBits::eTransferSrc);
		}

		auto pixel = m_need_stage
			             ? stage_buffer.memory.mapMemory(0, size, vk::MemoryMapFlags())
			             : memory.mapMemory(0, size, vk::MemoryMapFlags());
		memcpy(pixel, data, size);
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
				[&](vk::raii::CommandBuffer& cmd)
				{
					TransitionImageLayout(cmd, vk::ImageLayout::ePreinitialized,
					                      vk::ImageLayout::eShaderReadOnlyOptimal,
					                      vk::AccessFlagBits::eHostWrite,
					                      vk::AccessFlagBits::eShaderRead, vk::PipelineStageFlagBits::eHost,
					                      vk::PipelineStageFlagBits::eFragmentShader);
				});
		}
		GenerateMipMap();
		CreateSampler();
		CreateImageView();
	}

	void VulkanTexture2D::GenerateMipMap()
	{
		const auto& physical_device = VulkanContext::Get().physicalDevice;
		const auto format_props = physical_device.getFormatProperties(format);

		if (!(format_props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
		{
			LOG_ERROR("texture image format does not support linear blitting!");
			throw std::runtime_error("texture image format does not support linear blitting!");
		}
		auto ins = this;
		CommandExecutor::Get()
			.ImmediateExecute(
				VulkanContext::Get().graphicsQueue,
				[ins, mip_levels = m_info.mip_levels_](
				const vk::raii::CommandBuffer& cmd)
				{
					int32_t width = ins->m_info.extend_.width_, height = ins->m_info.extend_.height_;

					vk::ImageMemoryBarrier barrier;
					vk::ImageSubresourceRange range;
					range.setAspectMask(vk::ImageAspectFlagBits::eColor)
					     .setBaseMipLevel(0)
					     .setLevelCount(1)
					     .setBaseArrayLayer(0)
					     .setLayerCount(1);
					barrier.setImage(*ins->image)
					       .setSubresourceRange(range);

					for (uint32_t i = 1; i < mip_levels; i++)
					{
						range.setBaseMipLevel(i - 1);

						barrier
							.setSubresourceRange(range)
							.setOldLayout(vk::ImageLayout::eUndefined)
							.setNewLayout(vk::ImageLayout::eTransferDstOptimal)
							.setSrcAccessMask({})
							.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

						cmd.pipelineBarrier(
							vk::PipelineStageFlagBits::eTransfer,
							vk::PipelineStageFlagBits::eTransfer, {}, {}, nullptr, barrier);

						vk::ImageSubresourceLayers src_layers;
						src_layers.setAspectMask(vk::ImageAspectFlagBits::eColor)
						          .setMipLevel(i - 1)
						          .setBaseArrayLayer(0)
						          .setLayerCount(1);
						vk::ImageSubresourceLayers dst_layers;
						dst_layers.setAspectMask(vk::ImageAspectFlagBits::eColor)
						          .setMipLevel(i)
						          .setBaseArrayLayer(0)
						          .setLayerCount(1);


						vk::ImageBlit blit{};
						blit.setSrcOffsets({
							    {
								    {0, 0, 0},
								    {(width >> i - 1), (height >> i - 1), 1}
							    }
						    })
						    .setSrcSubresource(src_layers)
						    .setDstOffsets({
							    {
								    {0, 0, 0},
								    {
									    (width >> i),
									    (height >> i), 1
								    }
							    }
						    }).setDstSubresource(dst_layers);

						cmd.blitImage(*ins->image, vk::ImageLayout::eTransferSrcOptimal,
						              *ins->image, vk::ImageLayout::eTransferDstOptimal,
						              blit, vk::Filter::eLinear);

						barrier
							.setOldLayout(vk::ImageLayout::eTransferDstOptimal)
							.setNewLayout(vk::ImageLayout::eTransferSrcOptimal)
							.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
							.setDstAccessMask(vk::AccessFlagBits::eTransferRead);

						cmd.pipelineBarrier(
							vk::PipelineStageFlagBits::eTransfer,
							vk::PipelineStageFlagBits::eTransfer, {}, {}, nullptr, barrier);
					}
					range.setBaseMipLevel(0);
					range.setLevelCount(mip_levels);
					barrier
						.setSubresourceRange(range);
					barrier
						.setOldLayout(vk::ImageLayout::eTransferSrcOptimal)
						.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
						.setSrcAccessMask(vk::AccessFlagBits::eTransferRead)
						.setDstAccessMask(vk::AccessFlagBits::eShaderRead);

					cmd.pipelineBarrier(
						vk::PipelineStageFlagBits::eTransfer,
						vk::PipelineStageFlagBits::eFragmentShader, {}, {}, nullptr, barrier);
				});
	}

	void VulkanTexture2D::CreateImageView()
	{
		const auto& device = VulkanContext::Get().device;
		vk::ImageSubresourceRange range;
		range.setAspectMask(vk::ImageAspectFlagBits::eColor)
		     .setBaseMipLevel(0)
		     .setLevelCount(m_info.mip_levels_)
		     .setBaseArrayLayer(0)
		     .setLayerCount(1);

		vk::ImageViewCreateInfo image_view_create_info;
		image_view_create_info
			.setFlags({})
			.setFormat(format)
			.setImage(*image)
			.setViewType(vk::ImageViewType::e2D)
			.setSubresourceRange(range)
			.setComponents({});

		imageView = vk::raii::ImageView(device, image_view_create_info);
	}

	void VulkanTexture2D::CreateSampler()
	{
		const auto& device = VulkanContext::Get().device;
		vk::SamplerCreateInfo sampler_create_info;
		sampler_create_info
			.setFlags({})
			.setMagFilter(Utils::ToVulkanFilter(m_info.sampler_filter_))
			.setMinFilter(Utils::ToVulkanFilter(m_info.sampler_filter_))
			.setMipmapMode(vk::SamplerMipmapMode::eLinear)
			.setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
			.setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
			.setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
			.setMipLodBias(0.0f)
			.setAnisotropyEnable(false)
			.setCompareEnable(false)
			.setMaxAnisotropy(1.0f)
			.setCompareOp(vk::CompareOp::eNever)
			.setMinLod(0.0f)
			.setMaxLod((m_need_stage) ? static_cast<float>(m_info.mip_levels_) : 0.0f)
			.setBorderColor(vk::BorderColor::eFloatOpaqueWhite);

		sampler = vk::raii::Sampler(device, sampler_create_info);
	}

	void VulkanTexture2D::TransitionImageLayout(const vk::raii::CommandBuffer& cmdBuf, vk::ImageLayout lodLay,
	                                            vk::ImageLayout newLay,
	                                            vk::AccessFlagBits srcMask, vk::AccessFlagBits dstMask,
	                                            vk::PipelineStageFlagBits srcPipMask,
	                                            vk::PipelineStageFlagBits dstPipMask)
	{
		vk::ImageMemoryBarrier barrier;
		vk::ImageSubresourceRange range;
		range
			.setAspectMask(vk::ImageAspectFlagBits::eColor)
			.setBaseMipLevel(0)
			.setLevelCount(1)
			.setBaseArrayLayer(0)
			.setLayerCount(1);
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

	void VulkanTexture2D::CopyDataToImage(vk::raii::CommandBuffer& cmd, const vk::raii::Buffer& buffer, uint32_t width,
	                                      uint32_t height)
	{
		vk::BufferImageCopy buffer_copy_regions;
		buffer_copy_regions
			.setBufferImageHeight(0)
			.setBufferOffset(0)
			.setImageOffset({0, 0, 0})
			.setImageExtent({width, height, 1})
			.setBufferRowLength(0)
			.setImageSubresource({vk::ImageAspectFlagBits::eColor, 0, 0, 1});
		cmd.copyBufferToImage(*buffer, *image, vk::ImageLayout::eTransferDstOptimal, buffer_copy_regions);
	}

	void VulkanTexture2D::CreateImage(uint32_t width, uint32_t height)
	{
		vk::ImageCreateInfo create_info;
		create_info
			.setImageType(vk::ImageType::e2D)
			.setArrayLayers(1)
			.setMipLevels(m_info.mip_levels_)
			.setExtent({width, height, 1})
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

	void VulkanTexture2D::AllocateImageMemory()
	{
		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;

		const vk::MemoryRequirements memory_requirements = image.getMemoryRequirements();
		const uint32_t memoryTypeIndex = Utils::findMemoryType(
			physicalDevice.getMemoryProperties(),
			memory_requirements.memoryTypeBits,
			m_need_stage
				? vk::MemoryPropertyFlags()
				: (vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

		// allocate memory
		vk::MemoryAllocateInfo memory_allocate_info;
		memory_allocate_info.setAllocationSize(memory_requirements.size).setMemoryTypeIndex(memoryTypeIndex);
		memory = device.allocateMemory(memory_allocate_info);
	}
}
