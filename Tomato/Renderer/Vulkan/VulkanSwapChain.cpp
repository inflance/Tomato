#include "VulkanSwapChain.hpp"
#include <glfw/glfw3.h>

#include "VulkanContext.hpp"

namespace Tomato
{
	VulkanSwapChain::VulkanSwapChain(Window* window, uint32_t width, uint32_t height)
		: extent({width, height}), window(window)
	{
		InitSurface(window, width, height);
	}

	void VulkanSwapChain::InitSurface(const Window* window, uint32_t width, uint32_t height)
	{
		LOG_TRACE("Init Surface");
		const auto& instance = VulkanContext::Get().instance;
		VkSurfaceKHR _surface;
		const VkResult err = glfwCreateWindowSurface(*instance, static_cast<GLFWwindow*>(window->GetNativeWindow()),
		                                             nullptr,
		                                             &_surface);
		if (err != VK_SUCCESS)
			throw std::runtime_error("Failed to create window!");
		surface = vk::raii::SurfaceKHR(instance, _surface);
	}


	void VulkanSwapChain::InitSwapChain(uint32_t width, uint32_t height)
	{
		LOG_TRACE("Init Swap Chain width:{} height{}", width, height);
		extent.setWidth(width).setHeight(height);
		if (!images.empty())images.clear();
		if (!imageViews.empty())imageViews.clear();

		const auto& device = VulkanContext::Get().device;
		const auto& physicalDevice = VulkanContext::Get().physicalDevice;
		const auto& queueFamily = VulkanContext::Get().queueFamily;

		ASSERT(queueFamily.IsComplete(), "queueFamily must Comlete!");

		vk::SurfaceFormatKHR surfaceFormat = Utils::pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(*surface));
		colorFormat = surfaceFormat.format;

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
		vk::Extent2D swapchainExtent;
		if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
		{
			// If the surface size is undefined, the size is set to the size of the images requested.
			swapchainExtent.width = std::clamp(extent.width, surfaceCapabilities.minImageExtent.width,
			                                   surfaceCapabilities.maxImageExtent.width);
			swapchainExtent.height = std::clamp(extent.height, surfaceCapabilities.minImageExtent.height,
			                                    surfaceCapabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			swapchainExtent = surfaceCapabilities.currentExtent;
		}
		vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms &
			                                               vk::SurfaceTransformFlagBitsKHR::eIdentity)
			                                               ? vk::SurfaceTransformFlagBitsKHR::eIdentity
			                                               : surfaceCapabilities.currentTransform;
		vk::CompositeAlphaFlagBitsKHR compositeAlpha =
			(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
				? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
				: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
				? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
				: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
				? vk::CompositeAlphaFlagBitsKHR::eInherit
				: vk::CompositeAlphaFlagBitsKHR::eOpaque;
		vk::PresentModeKHR presentMode = Utils::pickPresentMode(physicalDevice.getSurfacePresentModesKHR(*surface));


		//// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = surfaceCapabilities.minImageCount + 1;
		if ((surfaceCapabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfaceCapabilities.
			maxImageCount))
		{
			desiredNumberOfSwapchainImages = surfaceCapabilities.maxImageCount;
		}


		vk::SwapchainCreateInfoKHR swapChainCreateInfo({},
		                                               *surface,
		                                               desiredNumberOfSwapchainImages,
		                                               colorFormat,
		                                               surfaceFormat.colorSpace,
		                                               swapchainExtent,
		                                               1,
		                                               vk::ImageUsageFlagBits::eColorAttachment |
		                                               vk::ImageUsageFlagBits::eTransferSrc,
		                                               vk::SharingMode::eExclusive,
		                                               {},
		                                               preTransform,
		                                               compositeAlpha,
		                                               presentMode,
		                                               true,
		                                               pOldSwapChain ? **pOldSwapChain : nullptr);
		if (queueFamily.Graphics.value() != queueFamily.Present.value())
		{
			uint32_t queueFamilyIndices[2] = {queueFamily.Graphics.value(), queueFamily.Present.value()};
			// If the graphics and present queues are from different queue families, we either have to explicitly
			// transfer ownership of images between the queues, or we have to create the swapchain with imageSharingMode
			// as vk::SharingMode::eConcurrent
			swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			swapChainCreateInfo.queueFamilyIndexCount = 2;
			swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		swapChain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);

		images = swapChain.getImages();

		imageViews.reserve(images.size());
		vk::ImageViewCreateInfo imageViewCreateInfo({}, {}, vk::ImageViewType::e2D, colorFormat, {},
		                                            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
		for (auto image : images)
		{
			imageViewCreateInfo.image = image;
			imageViews.emplace_back(device, imageViewCreateInfo);
		}
		imageCount = imageViews.size();
		pOldSwapChain = &swapChain;
	}
}
