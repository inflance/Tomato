#pragma once

#include "../IndexBuffer.h"
#include "vulkan/vulkan_raii.hpp"

namespace Tomato
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		~VulkanIndexBuffer() override;

		void Bind() const override
		{
		}

		void UnBind() const override
		{
		}

		[[nodiscard]] uint32_t GetCount() const override { return m_count; }

		vk::raii::Buffer m_index_buffer = nullptr;
		vk::raii::DeviceMemory m_device_memory = nullptr;

	private:
		uint32_t m_count{};
	};
}
