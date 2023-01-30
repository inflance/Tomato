#include "VulkanFence.h"

#include "VulkanDevice.h"
#include "VulkanUtils.h"

namespace Tomato
{
	VulkanFence::VulkanFence(bool createSignaled /*= false*/)
	{
		
	}

	VulkanFence::~VulkanFence()
	{
		
	}

	void VulkanFence::Destroy()
	{
	}

	bool VulkanFence::CheckState()
	{
		return false;
	}

	bool VulkanFence::IsSignaled()
	{
		return m_signaled ? m_signaled : CheckState();
	}

	bool VulkanFence::Wait()
	{
		return false;
	}

	void VulkanFence::Reset()
	{
	}

	void VulkanFence::WaitAndReset()
	{
		
	}
}
