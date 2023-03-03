#pragma once

#include <deque>

#include "Tomato/Renderer/Renderer.hpp"
#include "Tomato/Core/Window.h"
#include "Vulkan.hpp"

#include "VulkanUtils.hpp"
#include "VulkanStruct.hpp"
#include "Tomato/Renderer/GraphicsContext.hpp"

namespace Tomato
{
	class VulkanSwapChain;
	class VulkanVertexBuffer;
	class VulkanCommandBuffer;
	class VulkanPipeline;

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(Window* window);
		~VulkanContext() override;

		static auto& Get() { return *As<VulkanContext>(Engine::GetContext()); }
		void Begin() override;
		void Init() override;
		void CreateSwapChain();
		void SwapChainResize(uint32_t width, uint32_t height);
		void Destroy() override;
		void Present() override;
		QueueFamily FindQueueFamily();
		void OnResize(uint32_t width, uint32_t height) override;
		const auto& GetCurrentCommandBuffer() const { return commandBuffers[cur_frame]; }

		const auto& GetCurrentFrameBuffer() const
		{
			return frameBuffers[imageIndex];
		}

		Window* m_window_handle = nullptr;
		uint32_t m_width, m_height;

		vk::raii::Context context;
		vk::raii::Instance instance = nullptr;
		vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger = nullptr;
		vk::raii::PhysicalDevice physicalDevice = nullptr;
		vk::raii::Device device = nullptr;
		vk::raii::CommandPool commandPool = nullptr;
		vk::raii::CommandBuffers commandBuffers = nullptr;
		vk::raii::Queue graphicsQueue = nullptr;
		vk::raii::Queue presentQueue = nullptr;

		std::vector<std::string> instanceExt;
		std::vector<std::string> requiredLay;
		std::vector<const char*> enabledExtensions;
		std::vector<const char*> enabledLayers;
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties;

		//Todo remove the class
		QueueFamily queueFamily;

		Ref<VulkanSwapChain> swapChain = nullptr;

		Utils::DepthBufferData m_depth_buffer_data = nullptr;
		Ref<VulkanVertexBuffer> vertex_buffer;
		uint32_t imageIndex{};
		uint32_t cur_frame{};

		vk::raii::PipelineCache pipelineCache = nullptr;

		vk::raii::RenderPass renderPass = nullptr;

		std::vector<vk::raii::Fence> drawFences;
		std::vector<vk::raii::Semaphore> imageAcquiredSemaphore;
		std::vector<vk::raii::Framebuffer> frameBuffers;
	};
}
