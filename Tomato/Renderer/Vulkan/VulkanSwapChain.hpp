#pragma once

#include "vulkan/vulkan_raii.hpp"

#include "VulkanStruct.hpp"
#include "Tomato/Renderer/SwapChain.hpp"

namespace Tomato
{
	class Window;

	class VulkanSwapChain : public SwapChain
	{
	public:
		VulkanSwapChain(Window* window, uint32_t width, uint32_t height);

		void InitSurface(const Window* window, uint32_t width, uint32_t height) override;

		void InitSwapChain(uint32_t width, uint32_t height) override;

		auto GetImageCount() const { return imageCount; }

		auto operator*() const
		{
			return *swapChain;
		}

		vk::Extent2D extent;
		Window* window = nullptr;
		vk::raii::SurfaceKHR surface = nullptr;


		vk::Format colorFormat;
		vk::raii::SwapchainKHR* pOldSwapChain = nullptr;
		vk::raii::SwapchainKHR swapChain = nullptr;
		std::vector<vk::Image> images;
		std::vector<vk::raii::ImageView> imageViews;
		uint32_t imageCount = {};
	};
}
