#pragma once

#include <numeric>

#include "Vulkan.hpp"
#include "Tomato/Core/Core.hpp"
#include "Tomato/Renderer/Texture.hpp"
#include "GLFW/glfw3.h"

namespace Tomato
{
	class Window;
#define VK_CHECK_RESULT(stat) LOG_ASSERT((stat) == VK_SUCCESS, "Vulkan: Check  Error");

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	namespace Utils
	{
		static std::vector<const char*> GetEnableLayers(const std::vector<std::string>& layers
		                                                , const std::vector<vk::LayerProperties>& layerProperties)
		{
			std::vector<const char*> enabledLayers;
			enabledLayers.reserve(layers.size());
			for (const auto& layer : layers)
			{
				ASSERT(std::ranges::find_if(layerProperties.begin(), layerProperties.end(), [layer](
						const vk::LayerProperties& lp) { return layer == lp.layerName; }) !=
					layerProperties.end());
				enabledLayers.push_back(layer.data());
			}

			if (std::ranges::find(layers.begin(), layers.end(), "VK_LAYER_KHRONOS_validation") == layers.end() &&
				std::ranges::find_if(layerProperties.begin(),
				                     layerProperties.end(),
				                     [](const vk::LayerProperties& lp)
				                     {
					                     return ("VK_LAYER_KHRONOS_validation" <=> lp.layerName == nullptr);
				                     }) != layerProperties.end())
			{
				enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
			}
			return enabledLayers;
		}

		static std::vector<const char*> gatherExtensions(const std::vector<std::string>& extensions
		                                                 ,
		                                                 const std::vector<vk::ExtensionProperties>& extensionProperties
		)
		{
			std::vector<const char*> enabledExtensions;
			enabledExtensions.reserve(extensions.size());
			for (const auto& ext : extensions)
			{
				assert(std::ranges::find_if(extensionProperties.begin(),
						extensionProperties.end(),
						[ext](const vk::ExtensionProperties& ep) { return ext == ep.extensionName; }) !=
					extensionProperties
					.end());
				enabledExtensions.push_back(ext.data());
			}
			if (std::ranges::find(extensions.begin(), extensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == extensions
				.end()
				&&
				std::ranges::find_if(extensionProperties.begin(),
				                     extensionProperties.end(),
				                     [](const vk::ExtensionProperties& ep)
				                     {
					                     return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0);
				                     }) != extensionProperties.end())
			{
				enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}
			return enabledExtensions;
		}

		static inline uint32_t findMemoryType(const vk::PhysicalDeviceMemoryProperties& memoryProperties,
		                                      uint32_t typeBits,
		                                      vk::MemoryPropertyFlags requirementsMask)
		{
			uint32_t typeIndex = static_cast<uint32_t>(~0);
			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
			{
				if ((typeBits & 1) && ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) ==
					requirementsMask))
				{
					typeIndex = i;
					break;
				}
				typeBits >>= 1;
			}
			assert(typeIndex != static_cast<uint32_t>(~0));
			return typeIndex;
		}

		static inline vk::raii::DeviceMemory allocateDeviceMemory(const vk::raii::Device& device,
		                                                          const vk::PhysicalDeviceMemoryProperties&
		                                                          memoryProperties,
		                                                          const vk::MemoryRequirements& memoryRequirements,
		                                                          vk::MemoryPropertyFlags memoryPropertyFlags)
		{
			uint32_t memoryTypeIndex = findMemoryType(memoryProperties, memoryRequirements.memoryTypeBits,
			                                          memoryPropertyFlags);
			vk::MemoryAllocateInfo memoryAllocateInfo(memoryRequirements.size, memoryTypeIndex);
			return vk::raii::DeviceMemory(device, memoryAllocateInfo);
		}

		struct BufferData
		{
			BufferData(const vk::raii::PhysicalDevice& physicalDevice,
			           const vk::raii::Device& device,
			           vk::DeviceSize size,
			           vk::BufferUsageFlags usage,
			           vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible |
				           vk::MemoryPropertyFlagBits::eHostCoherent)
				: buffer(device, vk::BufferCreateInfo({}, size, usage))
				  , deviceMemory(allocateDeviceMemory(device, physicalDevice.getMemoryProperties(), buffer.getMemoryRequirements(), propertyFlags))
#if !defined( NDEBUG )
				  , m_size(size)
				  , m_usage(usage)
				  , m_propertyFlags(propertyFlags)
#endif
			{
				buffer.bindMemory(*deviceMemory, 0);
			}

			BufferData(std::nullptr_t)
			{
			}

			template <typename DataType>
			void upload(const DataType& data) const
			{
				assert(
					(m_propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent) && (m_propertyFlags & vk::
						MemoryPropertyFlagBits::eHostVisible));
				assert(sizeof(DataType) <= m_size);

				void* dataPtr = deviceMemory.mapMemory(0, sizeof(DataType));
				memcpy(dataPtr, &data, sizeof(DataType));
				deviceMemory.unmapMemory();
			}

			template <typename DataType>
			void upload(const std::vector<DataType>& data, size_t stride = 0) const
			{
				assert(m_propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible);

				size_t elementSize = stride ? stride : sizeof(DataType);
				assert(sizeof(DataType) <= elementSize);

				copyToDevice(deviceMemory, data.data(), data.size(), elementSize);
			}

			template <typename DataType>
			void upload(const vk::raii::PhysicalDevice& physicalDevice,
			            const vk::raii::Device& device,
			            const vk::raii::CommandPool& commandPool,
			            const vk::raii::Queue& queue,
			            const std::vector<DataType>& data,
			            size_t stride) const
			{
				assert(m_usage & vk::BufferUsageFlagBits::eTransferDst);
				assert(m_propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal);

				size_t elementSize = stride ? stride : sizeof(DataType);
				assert(sizeof(DataType) <= elementSize);

				size_t dataSize = data.size() * elementSize;
				assert(dataSize <= m_size);

				BufferData stagingBuffer(physicalDevice, device, dataSize, vk::BufferUsageFlagBits::eTransferSrc);
				copyToDevice(stagingBuffer.deviceMemory, data.data(), data.size(), elementSize);

				oneTimeSubmit(device,
				              commandPool,
				              queue,
				              [&](const vk::raii::CommandBuffer& commandBuffer)
				              {
					              commandBuffer.copyBuffer(*stagingBuffer.buffer, *this->buffer,
					                                       vk::BufferCopy(0, 0, dataSize));
				              });
			}

			// the order of buffer and deviceMemory here is important to get the constructor running !
			vk::raii::Buffer buffer = nullptr;
			vk::raii::DeviceMemory deviceMemory = nullptr;
#if !defined( NDEBUG )

		private:
			vk::DeviceSize m_size;
			vk::BufferUsageFlags m_usage;
			vk::MemoryPropertyFlags m_propertyFlags;
#endif
		};

		static void setImageLayout(
			const vk::raii::CommandBuffer& commandBuffer, vk::Image image, vk::Format format,
			vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout)
		{
			vk::AccessFlags sourceAccessMask;
			switch (oldImageLayout)
			{
			case vk::ImageLayout::eTransferDstOptimal: sourceAccessMask = vk::AccessFlagBits::eTransferWrite;
				break;
			case vk::ImageLayout::ePreinitialized: sourceAccessMask = vk::AccessFlagBits::eHostWrite;
				break;
			case vk::ImageLayout::eGeneral: // sourceAccessMask is empty
			case vk::ImageLayout::eUndefined: break;
			default: assert(false);
				break;
			}

			vk::PipelineStageFlags sourceStage;
			switch (oldImageLayout)
			{
			case vk::ImageLayout::eGeneral:
			case vk::ImageLayout::ePreinitialized: sourceStage = vk::PipelineStageFlagBits::eHost;
				break;
			case vk::ImageLayout::eTransferDstOptimal: sourceStage = vk::PipelineStageFlagBits::eTransfer;
				break;
			case vk::ImageLayout::eUndefined: sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
				break;
			default: assert(false);
				break;
			}

			vk::AccessFlags destinationAccessMask;
			switch (newImageLayout)
			{
			case vk::ImageLayout::eColorAttachmentOptimal: destinationAccessMask =
					vk::AccessFlagBits::eColorAttachmentWrite;
				break;
			case vk::ImageLayout::eDepthStencilAttachmentOptimal:
				destinationAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead |
					vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				break;
			case vk::ImageLayout::eGeneral: // empty destinationAccessMask
			case vk::ImageLayout::ePresentSrcKHR: break;
			case vk::ImageLayout::eShaderReadOnlyOptimal: destinationAccessMask = vk::AccessFlagBits::eShaderRead;
				break;
			case vk::ImageLayout::eTransferSrcOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferRead;
				break;
			case vk::ImageLayout::eTransferDstOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferWrite;
				break;
			default: assert(false);
				break;
			}

			vk::PipelineStageFlags destinationStage;
			switch (newImageLayout)
			{
			case vk::ImageLayout::eColorAttachmentOptimal: destinationStage =
					vk::PipelineStageFlagBits::eColorAttachmentOutput;
				break;
			case vk::ImageLayout::eDepthStencilAttachmentOptimal: destinationStage =
					vk::PipelineStageFlagBits::eEarlyFragmentTests;
				break;
			case vk::ImageLayout::eGeneral: destinationStage = vk::PipelineStageFlagBits::eHost;
				break;
			case vk::ImageLayout::ePresentSrcKHR: destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe;
				break;
			case vk::ImageLayout::eShaderReadOnlyOptimal: destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
				break;
			case vk::ImageLayout::eTransferDstOptimal:
			case vk::ImageLayout::eTransferSrcOptimal: destinationStage = vk::PipelineStageFlagBits::eTransfer;
				break;
			default: assert(false);
				break;
			}

			vk::ImageAspectFlags aspectMask;
			if (newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
			{
				aspectMask = vk::ImageAspectFlagBits::eDepth;
				if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
				{
					aspectMask |= vk::ImageAspectFlagBits::eStencil;
				}
			}
			else
			{
				aspectMask = vk::ImageAspectFlagBits::eColor;
			}

			vk::ImageSubresourceRange imageSubresourceRange(aspectMask, 0, 1, 0, 1);
			vk::ImageMemoryBarrier imageMemoryBarrier(sourceAccessMask,
			                                          destinationAccessMask,
			                                          oldImageLayout,
			                                          newImageLayout,
			                                          VK_QUEUE_FAMILY_IGNORED,
			                                          VK_QUEUE_FAMILY_IGNORED,
			                                          image,
			                                          imageSubresourceRange);
			return commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr,
			                                     imageMemoryBarrier);
		}

		struct ImageData
		{
			ImageData(const vk::raii::PhysicalDevice& physicalDevice,
			          const vk::raii::Device& device,
			          vk::Format format_,
			          const vk::Extent2D& extent,
			          vk::ImageTiling tiling,
			          vk::ImageUsageFlags usage,
			          vk::ImageLayout initialLayout,
			          vk::MemoryPropertyFlags memoryProperties,
			          vk::ImageAspectFlags aspectMask)
				: format(format_)
				  , image(device,
				          {
					          vk::ImageCreateFlags(),
					          vk::ImageType::e2D,
					          format,
					          vk::Extent3D(extent, 1),
					          1,
					          1,
					          vk::SampleCountFlagBits::e1,
					          tiling,
					          usage | vk::ImageUsageFlagBits::eSampled,
					          vk::SharingMode::eExclusive,
					          {},
					          initialLayout
				          })
				  , deviceMemory(allocateDeviceMemory(device, physicalDevice.getMemoryProperties(), image.getMemoryRequirements(), memoryProperties))
			{
				image.bindMemory(*deviceMemory, 0);
				imageView = vk::raii::ImageView(
					device, vk::ImageViewCreateInfo({}, *image, vk::ImageViewType::e2D, format, {},
					                                {aspectMask, 0, 1, 0, 1}));
			}

			ImageData(std::nullptr_t)
			{
			}

			vk::Format format;
			vk::raii::Image image = nullptr;
			vk::raii::DeviceMemory deviceMemory = nullptr;
			vk::raii::ImageView imageView = nullptr;
		};

		struct DepthBufferData : public ImageData
		{
			DepthBufferData(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::Device& device,
			                vk::Format format, const vk::Extent2D& extent) : ImageData(physicalDevice,
				device,
				format,
				extent,
				vk::ImageTiling::eOptimal,
				vk::ImageUsageFlagBits::eDepthStencilAttachment,
				vk::ImageLayout::eUndefined,
				vk::MemoryPropertyFlagBits::eDeviceLocal,
				vk::ImageAspectFlagBits::eDepth)
			{
			}

			DepthBufferData(std::nullptr_t) : ImageData(nullptr)
			{
			};
		};

		struct TextureData
		{
			TextureData(const vk::raii::PhysicalDevice& physicalDevice,
			            const vk::raii::Device& device,
			            const vk::Extent2D& extent_ = {256, 256},
			            vk::ImageUsageFlags usageFlags = {},
			            vk::FormatFeatureFlags formatFeatureFlags = {},
			            bool anisotropyEnable = false,
			            bool forceStaging = false)
				: format(vk::Format::eR8G8B8A8Unorm)
				  , extent(extent_)
				  , sampler(device,
				            {
					            {},
					            vk::Filter::eLinear,
					            vk::Filter::eLinear,
					            vk::SamplerMipmapMode::eLinear,
					            vk::SamplerAddressMode::eRepeat,
					            vk::SamplerAddressMode::eRepeat,
					            vk::SamplerAddressMode::eRepeat,
					            0.0f,
					            anisotropyEnable,
					            16.0f,
					            false,
					            vk::CompareOp::eNever,
					            0.0f,
					            0.0f,
					            vk::BorderColor::eFloatOpaqueBlack
				            })
			{
				vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(format);

				formatFeatureFlags |= vk::FormatFeatureFlagBits::eSampledImage;
				needsStaging = forceStaging || ((formatProperties.linearTilingFeatures & formatFeatureFlags) !=
					formatFeatureFlags);
				vk::ImageTiling imageTiling;
				vk::ImageLayout initialLayout;
				vk::MemoryPropertyFlags requirements;
				if (needsStaging)
				{
					assert((formatProperties.optimalTilingFeatures & formatFeatureFlags) == formatFeatureFlags);
					stagingBufferData = BufferData(physicalDevice, device, extent.width * extent.height * 4,
					                               vk::BufferUsageFlagBits::eTransferSrc);
					imageTiling = vk::ImageTiling::eOptimal;
					usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
					initialLayout = vk::ImageLayout::eUndefined;
				}
				else
				{
					imageTiling = vk::ImageTiling::eLinear;
					initialLayout = vk::ImageLayout::ePreinitialized;
					requirements = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
				}
				imageData = ImageData(physicalDevice,
				                      device,
				                      format,
				                      extent,
				                      imageTiling,
				                      usageFlags | vk::ImageUsageFlagBits::eSampled,
				                      initialLayout,
				                      requirements,
				                      vk::ImageAspectFlagBits::eColor);
			}

			template <typename ImageGenerator>
			void setImage(const vk::raii::CommandBuffer& commandBuffer, const ImageGenerator& imageGenerator)
			{
				void* data = needsStaging
					             ? stagingBufferData.deviceMemory.mapMemory(
						             0, stagingBufferData.buffer.getMemoryRequirements().size)
					             : imageData.deviceMemory.mapMemory(0, imageData.image.getMemoryRequirements().size);
				imageGenerator(data, extent);
				needsStaging ? stagingBufferData.deviceMemory.unmapMemory() : imageData.deviceMemory.unmapMemory();

				if (needsStaging)
				{
					// Since we're going to blit to the texture image, set its layout to eTransferDstOptimal
					setImageLayout(
						commandBuffer, *imageData.image, imageData.format, vk::ImageLayout::eUndefined,
						vk::ImageLayout::eTransferDstOptimal);
					vk::BufferImageCopy copyRegion(0,
					                               extent.width,
					                               extent.height,
					                               vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
					                               vk::Offset3D(0, 0, 0),
					                               vk::Extent3D(extent, 1));
					commandBuffer.copyBufferToImage(*stagingBufferData.buffer, *imageData.image,
					                                vk::ImageLayout::eTransferDstOptimal, copyRegion);
					// Set the layout for the texture image from eTransferDstOptimal to eShaderReadOnlyOptimal
					setImageLayout(
						commandBuffer, *imageData.image, imageData.format, vk::ImageLayout::eTransferDstOptimal,
						vk::ImageLayout::eShaderReadOnlyOptimal);
				}
				else
				{
					// If we can use the linear tiled image as a texture, just do it
					setImageLayout(
						commandBuffer, *imageData.image, imageData.format, vk::ImageLayout::ePreinitialized,
						vk::ImageLayout::eShaderReadOnlyOptimal);
				}
			}

			vk::Format format;
			vk::Extent2D extent;
			bool needsStaging;
			BufferData stagingBufferData = nullptr;
			ImageData imageData = nullptr;
			vk::raii::Sampler sampler;
		};

		static void updateDescriptorSets(
			const vk::raii::Device& device,
			const vk::raii::DescriptorSet& descriptorSet,
			const std::vector<std::tuple<vk::DescriptorType, const vk::raii::Buffer&, vk::DeviceSize, const
			                             vk::raii::BufferView*>>& bufferData,
			const TextureData& textureData,
			uint32_t bindingOffset = 0)
		{
			std::vector<vk::DescriptorBufferInfo> bufferInfos;
			bufferInfos.reserve(bufferData.size());

			std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
			writeDescriptorSets.reserve(bufferData.size() + 1);
			uint32_t dstBinding = bindingOffset;
			for (const auto& bd : bufferData)
			{
				bufferInfos.emplace_back(*std::get<1>(bd), 0, std::get<2>(bd));
				vk::BufferView bufferView;
				if (std::get<3>(bd))
				{
					bufferView = **std::get<3>(bd);
				}
				writeDescriptorSets.emplace_back(
					*descriptorSet, dstBinding++, 0, 1, std::get<0>(bd), nullptr, &bufferInfos.back(),
					std::get<3>(bd) ? &bufferView : nullptr);
			}

			vk::DescriptorImageInfo imageInfo(*textureData.sampler, *textureData.imageData.imageView,
			                                  vk::ImageLayout::eShaderReadOnlyOptimal);
			writeDescriptorSets.emplace_back(*descriptorSet, dstBinding, 0, vk::DescriptorType::eCombinedImageSampler,
			                                 imageInfo, nullptr, nullptr);

			device.updateDescriptorSets(writeDescriptorSets, nullptr);
		}

		static vk::raii::DescriptorPool makeDescriptorPool(const vk::raii::Device& device,
		                                                   const std::vector<vk::DescriptorPoolSize>& poolSizes)
		{
			assert(!poolSizes.empty());
			uint32_t maxSets = std::accumulate(
				poolSizes.begin(), poolSizes.end(), 0, [](uint32_t sum, const vk::DescriptorPoolSize& dps)
				{
					return sum + dps.descriptorCount;
				});
			assert(0 < maxSets);

			vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			                                                      maxSets, poolSizes);
			return vk::raii::DescriptorPool(device, descriptorPoolCreateInfo);
		}

		static vk::raii::DescriptorSetLayout makeDescriptorSetLayout(const vk::raii::Device& device,
		                                                             const std::vector<std::tuple<
			                                                             vk::DescriptorType, uint32_t,
			                                                             vk::ShaderStageFlags>>& bindingData,
		                                                             vk::DescriptorSetLayoutCreateFlags flags = {})
		{
			std::vector<vk::DescriptorSetLayoutBinding> bindings(bindingData.size());
			for (size_t i = 0; i < bindingData.size(); i++)
			{
				bindings[i] = vk::DescriptorSetLayoutBinding(
					static_cast<uint32_t>(i), std::get<0>(bindingData[i]), std::get<1>(bindingData[i]),
					std::get<2>(bindingData[i]));
			}
			vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(flags, bindings);
			return vk::raii::DescriptorSetLayout(device, descriptorSetLayoutCreateInfo);
		}

		static vk::raii::Pipeline makeGraphicsPipeline(const vk::raii::Device& device,
		                                               const vk::raii::PipelineCache& pipelineCache,
		                                               const vk::raii::ShaderModule& vertexShaderModule,
		                                               const vk::SpecializationInfo* vertexShaderSpecializationInfo,
		                                               const vk::raii::ShaderModule& fragmentShaderModule,
		                                               const vk::SpecializationInfo* fragmentShaderSpecializationInfo,
		                                               uint32_t vertexStride,
		                                               const std::vector<std::pair<vk::Format, uint32_t>>&
		                                               vertexInputAttributeFormatOffset,
		                                               vk::FrontFace frontFace,
		                                               bool depthBuffered,
		                                               const vk::raii::PipelineLayout& pipelineLayout,
		                                               const vk::raii::RenderPass& renderPass)
		{
			std::array<vk::PipelineShaderStageCreateInfo, 2> pipelineShaderStageCreateInfos = {
				vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, *vertexShaderModule, "main",
				                                  vertexShaderSpecializationInfo),
				vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, *fragmentShaderModule, "main",
				                                  fragmentShaderSpecializationInfo)
			};

			std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;
			vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
			vk::VertexInputBindingDescription vertexInputBindingDescription(0, vertexStride);

			if (0 < vertexStride)
			{
				vertexInputAttributeDescriptions.reserve(vertexInputAttributeFormatOffset.size());
				for (uint32_t i = 0; i < vertexInputAttributeFormatOffset.size(); i++)
				{
					vertexInputAttributeDescriptions.emplace_back(i, 0, vertexInputAttributeFormatOffset[i].first,
					                                              vertexInputAttributeFormatOffset[i].second);
				}
				pipelineVertexInputStateCreateInfo.setVertexBindingDescriptions(vertexInputBindingDescription);
				pipelineVertexInputStateCreateInfo.setVertexAttributeDescriptions(vertexInputAttributeDescriptions);
			}

			vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
				vk::PipelineInputAssemblyStateCreateFlags(),
				vk::PrimitiveTopology::eTriangleList);

			vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
				vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

			vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
				vk::PipelineRasterizationStateCreateFlags(),
				false,
				false,
				vk::PolygonMode::eFill,
				vk::CullModeFlagBits::eNone,
				frontFace,
				false,
				0.0f,
				0.0f,
				0.0f,
				1.0f);

			vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo({}, vk::SampleCountFlagBits::e1);

			vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep,
			                                  vk::CompareOp::eAlways);
			vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
				vk::PipelineDepthStencilStateCreateFlags(), depthBuffered, depthBuffered, vk::CompareOp::eLessOrEqual,
				false, false, stencilOpState, stencilOpState);

			vk::ColorComponentFlags colorComponentFlags(
				vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
				vk::ColorComponentFlagBits::eA);
			vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(false,
				vk::BlendFactor::eZero,
				vk::BlendFactor::eZero,
				vk::BlendOp::eAdd,
				vk::BlendFactor::eZero,
				vk::BlendFactor::eZero,
				vk::BlendOp::eAdd,
				colorComponentFlags);
			vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
				vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, pipelineColorBlendAttachmentState,
				{{1.0f, 1.0f, 1.0f, 1.0f}});

			std::array<vk::DynamicState, 2> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
			vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
				vk::PipelineDynamicStateCreateFlags(), dynamicStates);

			vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlags(),
			                                                          pipelineShaderStageCreateInfos,
			                                                          &pipelineVertexInputStateCreateInfo,
			                                                          &pipelineInputAssemblyStateCreateInfo,
			                                                          nullptr,
			                                                          &pipelineViewportStateCreateInfo,
			                                                          &pipelineRasterizationStateCreateInfo,
			                                                          &pipelineMultisampleStateCreateInfo,
			                                                          &pipelineDepthStencilStateCreateInfo,
			                                                          &pipelineColorBlendStateCreateInfo,
			                                                          &pipelineDynamicStateCreateInfo,
			                                                          *pipelineLayout,
			                                                          *renderPass);

			return vk::raii::Pipeline(device, pipelineCache, graphicsPipelineCreateInfo);
		}

		static vk::raii::RenderPass makeRenderPass(const vk::raii::Device& device,
		                                           vk::Format colorFormat,
		                                           vk::Format depthFormat,
		                                           vk::AttachmentLoadOp loadOp = vk::AttachmentLoadOp::eClear,
		                                           vk::ImageLayout colorFinalLayout = vk::ImageLayout::ePresentSrcKHR)
		{
			std::vector<vk::AttachmentDescription> attachmentDescriptions;
			assert(colorFormat != vk::Format::eUndefined);
			attachmentDescriptions.emplace_back(vk::AttachmentDescriptionFlags(),
			                                    colorFormat,
			                                    vk::SampleCountFlagBits::e1,
			                                    loadOp,
			                                    vk::AttachmentStoreOp::eStore,
			                                    vk::AttachmentLoadOp::eDontCare,
			                                    vk::AttachmentStoreOp::eDontCare,
			                                    vk::ImageLayout::eUndefined,
			                                    colorFinalLayout);
			if (depthFormat != vk::Format::eUndefined)
			{
				attachmentDescriptions.emplace_back(vk::AttachmentDescriptionFlags(),
				                                    depthFormat,
				                                    vk::SampleCountFlagBits::e1,
				                                    loadOp,
				                                    vk::AttachmentStoreOp::eDontCare,
				                                    vk::AttachmentLoadOp::eDontCare,
				                                    vk::AttachmentStoreOp::eDontCare,
				                                    vk::ImageLayout::eUndefined,
				                                    vk::ImageLayout::eDepthStencilAttachmentOptimal);
			}
			vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);
			vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
			vk::SubpassDescription subpassDescription(vk::SubpassDescriptionFlags(),
			                                          vk::PipelineBindPoint::eGraphics,
			                                          {},
			                                          colorAttachment,
			                                          {},
			                                          (depthFormat != vk::Format::eUndefined)
				                                          ? &depthAttachment
				                                          : nullptr);
			vk::RenderPassCreateInfo renderPassCreateInfo(vk::RenderPassCreateFlags(), attachmentDescriptions,
			                                              subpassDescription);
			return vk::raii::RenderPass(device, renderPassCreateInfo);
		}

		inline static vk::PresentModeKHR pickPresentMode(const std::vector<vk::PresentModeKHR>& presentModes)
		{
			auto pickedMode = vk::PresentModeKHR::eFifo;
			for (const auto& presentMode : presentModes)
			{
				if (presentMode == vk::PresentModeKHR::eMailbox)
				{
					pickedMode = presentMode;
					break;
				}

				if (presentMode == vk::PresentModeKHR::eImmediate)
				{
					pickedMode = presentMode;
				}
			}
			return pickedMode;
		}

		inline static vk::SurfaceFormatKHR pickSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
		{
			assert(!formats.empty());
			vk::SurfaceFormatKHR pickedFormat = formats[0];
			if (formats.size() == 1)
			{
				if (formats[0].format == vk::Format::eUndefined)
				{
					pickedFormat.format = vk::Format::eB8G8R8A8Unorm;
					pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
				}
			}
			else
			{
				// request several formats, the first found will be used
				vk::Format requestedFormats[] = {
					vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm,
					vk::Format::eR8G8B8Unorm
				};
				auto requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
				for (size_t i = 0; i < sizeof(requestedFormats) / sizeof(requestedFormats[0]); i++)
				{
					vk::Format requestedFormat = requestedFormats[i];
					auto it = std::find_if(formats.begin(),
					                       formats.end(),
					                       [requestedFormat, requestedColorSpace](const vk::SurfaceFormatKHR& f)
					                       {
						                       return (f.format == requestedFormat) && (f.colorSpace ==
							                       requestedColorSpace);
					                       });
					if (it != formats.end())
					{
						pickedFormat = *it;
						break;
					}
				}
			}
			assert(pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);
			return pickedFormat;
		}


		template <typename Func>
		void oneTimeSubmit(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::Queue& queue, const Func& func)
		{
			commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
			func(commandBuffer);
			commandBuffer.end();
			vk::SubmitInfo submitInfo(nullptr, nullptr, *commandBuffer);
			queue.submit(submitInfo, nullptr);
			queue.waitIdle();
		}

		template <typename Func>
		void oneTimeSubmit(const vk::raii::Device& device, const vk::raii::CommandPool& commandPool,
		                   const vk::raii::Queue& queue, const Func& func)
		{
			vk::raii::CommandBuffers commandBuffers(device, {*commandPool, vk::CommandBufferLevel::ePrimary, 1});
			oneTimeSubmit(commandBuffers.front(), queue, func);
		}

		template <typename T>
		static void copyToDevice(const vk::raii::DeviceMemory& deviceMemory, const T* pData, size_t count,
		                         vk::DeviceSize stride = sizeof(T))
		{
			assert(sizeof(T) <= stride);
			auto deviceData = static_cast<uint8_t*>(deviceMemory.mapMemory(0, count * stride));
			if (stride == sizeof(T))
			{
				memcpy(deviceData, pData, count * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < count; i++)
				{
					memcpy(deviceData, &pData[i], sizeof(T));
					deviceData += stride;
				}
			}
			deviceMemory.unmapMemory();
		}

		template <typename T>
		void copyToDevice(const vk::raii::DeviceMemory& deviceMemory, const T& data)
		{
			copyToDevice<T>(deviceMemory, &data, 1);
		}

		struct SurfaceData
		{
			SurfaceData(const vk::raii::Instance& instance, Window* window, const vk::Extent2D& extent_);

			SurfaceData(std::nullptr_t)
			{
			}

			vk::Extent2D extent;
			Window* window = nullptr;
			vk::raii::SurfaceKHR surface = nullptr;
		};

		static inline uint32_t findGraphicsQueueFamilyIndex(
			const std::vector<vk::QueueFamilyProperties>& queueFamilyProperties)
		{
			// get the first index into queueFamiliyProperties which supports graphics
			auto graphicsQueueFamilyProperty =
				std::find_if(queueFamilyProperties.begin(),
				             queueFamilyProperties.end(),
				             [](const vk::QueueFamilyProperties& qfp)
				             {
					             return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
				             });
			assert(graphicsQueueFamilyProperty != queueFamilyProperties.end());
			return static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
		}


		constexpr auto ToVulkanImageFilter(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::Linear: return VK_FILTER_LINEAR;
			case TextureFilter::Nearest: return VK_FILTER_NEAREST;
			case TextureFilter::Cubic: return VK_FILTER_CUBIC_IMG;
			case TextureFilter::None: break;
			}
			LOG_ASSERT(false, "Unknown filter");
			return VK_FILTER_LINEAR;
		}

		constexpr auto ToVulkanImageWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::Repeat:
				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case TextureWrap::Clamp:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case TextureWrap::MirroredRepeat:
				return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			case TextureWrap::ClampToEdge:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			default:
				//LOG_ERROR("Unknown wrap type!");
				break;
			}
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		}

		static inline std::string VkObjectTypeToString(vk::ObjectType type)
		{
			return "";
		}

		static void updateDescriptorSets(
			const vk::raii::Device& device,
			const vk::raii::DescriptorSet& descriptorSet,
			const std::vector<std::tuple<vk::DescriptorType, const vk::raii::Buffer&, vk::DeviceSize, const
			                             vk::raii::BufferView*>>& bufferData,
			const std::vector<std::pair<const vk::raii::Sampler&, const vk::raii::ImageView&>>& textureData,
			uint32_t bindingOffset = 0)
		{
			std::vector<vk::DescriptorBufferInfo> bufferInfos;
			bufferInfos.reserve(bufferData.size());

			std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
			writeDescriptorSets.reserve(bufferData.size() + (textureData.empty() ? 0 : 1));
			uint32_t dstBinding = bindingOffset;
			for (const auto& bd : bufferData)
			{
				bufferInfos.emplace_back(*std::get<1>(bd), 0, std::get<2>(bd));
				vk::BufferView bufferView;
				if (std::get<3>(bd))
				{
					bufferView = **std::get<3>(bd);
				}
				writeDescriptorSets.emplace_back(
					*descriptorSet, dstBinding++, 0, 1, std::get<0>(bd), nullptr, &bufferInfos.back(),
					std::get<3>(bd) ? &bufferView : nullptr);
			}

			std::vector<vk::DescriptorImageInfo> imageInfos;
			if (!textureData.empty())
			{
				imageInfos.reserve(textureData.size());
				for (const auto& thd : textureData)
				{
					imageInfos.emplace_back(*thd.first, *thd.second,
					                        vk::ImageLayout::eShaderReadOnlyOptimal);
				}
				writeDescriptorSets.emplace_back(*descriptorSet,
				                                 dstBinding,
				                                 0,
				                                 static_cast<uint32_t>(imageInfos.size()),
				                                 vk::DescriptorType::eCombinedImageSampler,
				                                 imageInfos.data(),
				                                 nullptr,
				                                 nullptr);
			}

			device.updateDescriptorSets(writeDescriptorSets, nullptr);
		}


		inline vk::Bool32 GetSupportedDepthFormat(const vk::raii::PhysicalDevice& physicalDevice,
		                                          vk::Format* depthFormat)
		{
			// Since all depth formats may be optional, we need to find a suitable depth format to use
			// Start with the highest precision packed format
			std::vector<vk::Format> depthFormats = {
				vk::Format::eD32SfloatS8Uint,
				vk::Format::eD32Sfloat,
				vk::Format::eD24UnormS8Uint,
				vk::Format::eD16UnormS8Uint,
				vk::Format::eD16Unorm
			};

			for (auto& format : depthFormats)
			{
				auto props = physicalDevice.getFormatProperties(format);
				// Format must support depth stencil attachment for optimal tiling
				if (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
				{
					*depthFormat = format;
					return true;
				}
			}

			return false;
		}
	}
}
