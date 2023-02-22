#pragma once
#include "VulkanUtils.h"
#include "vulkan/vulkan_raii.hpp"

namespace Tomato
{
	struct QueueFamily
	{
		std::optional<uint32_t> Graphics{};
		std::optional<uint32_t> Present{};

		bool IsComplete() const
		{
			return Graphics.has_value() && Present.has_value();
		}
	};

	struct Buffer
	{
		Buffer(const vk::raii::PhysicalDevice& physicalDevice,
		       const vk::raii::Device& device,
		       vk::DeviceSize size,
		       vk::BufferUsageFlags usage,
		       vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible |
			       vk::MemoryPropertyFlagBits::eHostCoherent)
			: buffer(vk::raii::Buffer(device, vk::BufferCreateInfo({}, size, usage)))
			  , memory(Utils::allocateDeviceMemory(device, physicalDevice.getMemoryProperties(),
			                                       buffer.getMemoryRequirements(), propertyFlags))
			  , size(size), usage(usage)
		{
			buffer.bindMemory(*memory, 0);
		}

		Buffer(nullptr_t)
		{
		}

		vk::raii::Buffer buffer = nullptr;
		vk::raii::DeviceMemory memory = nullptr;
		vk::DeviceSize size{};
		vk::BufferUsageFlags usage{};
	};
}
