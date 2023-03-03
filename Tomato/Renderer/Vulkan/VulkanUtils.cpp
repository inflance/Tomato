#include "VulkanUtils.hpp"

#include "Tomato/Core/Window.h"

namespace Tomato::Utils
{
	SurfaceData::SurfaceData(const vk::raii::Instance& instance, Window* window, const vk::Extent2D& extent_)
		: extent(extent_), window(window)
	{
		VkSurfaceKHR _surface;
		VkResult err = glfwCreateWindowSurface(*instance, static_cast<GLFWwindow*>(window->GetNativeWindow()), nullptr,
		                                       &_surface);
		if (err != VK_SUCCESS)
			throw std::runtime_error("Failed to create window!");
		surface = vk::raii::SurfaceKHR(instance, _surface);
	}
}
