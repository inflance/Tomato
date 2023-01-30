#pragma once

#include <vulkan/vulkan.h>

namespace Tomato
{
	class VulkanFence
	{
	public:
		VulkanFence(bool createSignaled = false);
		~VulkanFence();

		void Destroy();

		bool CheckState();
		bool IsSignaled();
		bool Wait();
		void Reset();
		void WaitAndReset();

		void SetSignaled(bool signaled)
		{
			this->m_signaled = signaled;
		}

		auto Get()
		{
			return m_fence;
		}

	private:
		VkFence m_fence;
		bool m_signaled;
	};
}
