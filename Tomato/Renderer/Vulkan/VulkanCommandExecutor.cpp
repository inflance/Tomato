#include "VulkanCommandExecutor.hpp"

#include "VulkanContext.hpp"
#include "Tomato/Core/Timer.h"

namespace Tomato
{
	CommandExecutor::CommandExecutor()
	{
		m_pool = CreateCommandPool();
	}

	CommandExecutor::~CommandExecutor()
	{
	}

	vk::raii::CommandBuffer CommandExecutor::CreateACommandBuffer() const
	{
		const auto& commandPool = VulkanContext::Get().commandPool;
		const auto& device = VulkanContext::Get().device;
		vk::CommandBufferAllocateInfo cmdBufAllocateInfo;
		cmdBufAllocateInfo
			.setCommandPool(*commandPool)
			.setCommandBufferCount(1)
			.setLevel(vk::CommandBufferLevel::ePrimary);
		return std::move(device.allocateCommandBuffers(cmdBufAllocateInfo).front());
	}

	vk::raii::CommandBuffers CommandExecutor::CreateCommandBuffers(uint32_t count, bool second)
	{
		const auto& commandPool = VulkanContext::Get().commandPool;
		const auto& device = VulkanContext::Get().device;
		vk::CommandBufferAllocateInfo cmdBufAllocateInfo;
		cmdBufAllocateInfo
			.setCommandPool(*commandPool)
			.setCommandBufferCount(count)
			.setLevel(second ? vk::CommandBufferLevel::eSecondary : vk::CommandBufferLevel::ePrimary);
		return vk::raii::CommandBuffers(device, cmdBufAllocateInfo);
	}

	vk::raii::CommandPool CommandExecutor::CreateCommandPool()
	{
		const auto& context = VulkanContext::Get();
		vk::CommandPoolCreateInfo create_info;
		create_info.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
		           .setQueueFamilyIndex(context.queueFamily.Graphics.value());

		return vk::raii::CommandPool{context.device, create_info};
	}

	void CommandExecutor::ImmediateExecute(const vk::raii::Queue& queue, ExecuteCommandFn&& cmdFn) const
	{
		Timer timer;
		auto cmd = CreateACommandBuffer();
		vk::CommandBufferBeginInfo begin_info;
		begin_info.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		cmd.begin(begin_info);
		if (cmdFn) cmdFn(cmd);
		cmd.end();

		constexpr uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;
		vk::SubmitInfo submitInfo;
		submitInfo.setCommandBufferCount(1)
		          .setCommandBuffers(*cmd);

		const auto& device = VulkanContext::Get().device;
		const vk::raii::Fence fence(device, vk::FenceCreateInfo());
		queue.submit(submitInfo, *fence);
		queue.waitIdle();
		device.waitIdle();
		while (vk::Result::eTimeout == device.waitForFences({*fence}, VK_TRUE, DEFAULT_FENCE_TIMEOUT));
		cmd.reset({});
		LOG_INFO("ImmediateExecute {}ms", timer.CountTime());
	}
}
