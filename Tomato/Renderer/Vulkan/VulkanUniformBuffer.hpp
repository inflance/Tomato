#pragma once

#include "Vulkan.hpp"
#include "Tomato/Renderer/UniformBuffer.hpp"

namespace Tomato
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t binding = 0);
		~VulkanUniformBuffer() override;


		void SetData(const void* data, uint32_t size, uint32_t offset) override;

		uint32_t GetBinding() const override { return binding; };

		uint32_t m_size{}, binding{};
		vk::raii::Buffer uniformDataBuffer = nullptr;
		vk::raii::DeviceMemory uniformDataMemory = nullptr;
	};
}
