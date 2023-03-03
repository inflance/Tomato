#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "Tomato/Core/Singleton.h"

namespace Tomato
{
	class CommandExecutor final : public Singleton<CommandExecutor>
	{
	public:
		CommandExecutor();
		~CommandExecutor() override;

		[[nodiscard]] vk::raii::CommandBuffer CreateACommandBuffer() const;
		static [[nodiscard]] vk::raii::CommandBuffers CreateCommandBuffers(uint32_t count, bool second = false);

		using ExecuteCommandFn = std::function<void(vk::raii::CommandBuffer&)>;
		void ImmediateExecute(const vk::raii::Queue& queue, ExecuteCommandFn&& cmdFn) const;

	private:
		[[nodiscard]] static vk::raii::CommandPool CreateCommandPool();
		vk::raii::CommandPool m_pool = nullptr;
	};
}
