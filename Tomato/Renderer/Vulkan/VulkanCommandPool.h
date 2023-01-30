#pragma once

#include "vulkan/vulkan.h"

namespace Tomato
{
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(int32_t queue_index, VkCommandPoolCreateFlags flags);
		~VulkanCommandPool();

		void Destroy();

		void Reset() const;

		[[nodiscard]]inline auto Get() const
		{
			return m_command_pool;
		}

	private:
		VkCommandPool m_command_pool;
	};
}
