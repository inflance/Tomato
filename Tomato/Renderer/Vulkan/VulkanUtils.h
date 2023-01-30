#pragma once
#include "Vulkan.h"
#include "Tomato/Core/Core.h"

namespace Tomato
{
#define VK_CHECK_RESULT(stat, name) LOG_ASSERT((stat) == VK_SUCCESS, "Vulkan: Check {0} Error", name);

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	namespace Utils
	{

		inline void SetDebugUtilsObjectName(const VkDevice device, const VkObjectType objectType, std::string_view name, const void* handle)
		{
			VkDebugUtilsObjectNameInfoEXT nameInfo;
			nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
			nameInfo.objectType = objectType;
			nameInfo.pObjectName = name.data();
			nameInfo.objectHandle = (uint64_t)handle;
			nameInfo.pNext = VK_NULL_HANDLE;

			VK_CHECK_RESULT(fpSetDebugUtilsObjectNameEXT(device, &nameInfo),"");
		}

		inline void VulkanLoadDebugUtilsExtensions(VkInstance instance)
		{
			fpSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
			if (fpSetDebugUtilsObjectNameEXT == nullptr)
				fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) { return VK_SUCCESS; };

			fpCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
			if (fpCmdBeginDebugUtilsLabelEXT == nullptr)
				fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {};

			fpCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
			if (fpCmdEndDebugUtilsLabelEXT == nullptr)
				fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer) {};

			fpCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
			if (fpCmdInsertDebugUtilsLabelEXT == nullptr)
				fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {};
		}

		inline std::string_view VkObjectTypeToString(const VkObjectType type)
		{
			switch (type)
			{
			case VK_OBJECT_TYPE_COMMAND_BUFFER: return "VK_OBJECT_TYPE_COMMAND_BUFFER";
			case VK_OBJECT_TYPE_PIPELINE: return "VK_OBJECT_TYPE_PIPELINE";
			case VK_OBJECT_TYPE_FRAMEBUFFER: return "VK_OBJECT_TYPE_FRAMEBUFFER";
			case VK_OBJECT_TYPE_IMAGE: return "VK_OBJECT_TYPE_IMAGE";
			case VK_OBJECT_TYPE_QUERY_POOL: return "VK_OBJECT_TYPE_QUERY_POOL";
			case VK_OBJECT_TYPE_RENDER_PASS: return "VK_OBJECT_TYPE_RENDER_PASS";
			case VK_OBJECT_TYPE_COMMAND_POOL: return "VK_OBJECT_TYPE_COMMAND_POOL";
			case VK_OBJECT_TYPE_PIPELINE_CACHE: return "VK_OBJECT_TYPE_PIPELINE_CACHE";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV";
			case VK_OBJECT_TYPE_BUFFER: return "VK_OBJECT_TYPE_BUFFER";
			case VK_OBJECT_TYPE_BUFFER_VIEW: return "VK_OBJECT_TYPE_BUFFER_VIEW";
			case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT: return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
			case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT: return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
			case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR: return "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR";
			case VK_OBJECT_TYPE_DESCRIPTOR_POOL: return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET: return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT: return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
			case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
			case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT: return "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT";
			case VK_OBJECT_TYPE_DEVICE: return "VK_OBJECT_TYPE_DEVICE";
			case VK_OBJECT_TYPE_DEVICE_MEMORY: return "VK_OBJECT_TYPE_DEVICE_MEMORY";
			case VK_OBJECT_TYPE_PIPELINE_LAYOUT: return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
			case VK_OBJECT_TYPE_DISPLAY_KHR: return "VK_OBJECT_TYPE_DISPLAY_KHR";
			case VK_OBJECT_TYPE_DISPLAY_MODE_KHR: return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
			case VK_OBJECT_TYPE_PHYSICAL_DEVICE: return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
			case VK_OBJECT_TYPE_EVENT: return "VK_OBJECT_TYPE_EVENT";
			case VK_OBJECT_TYPE_FENCE: return "VK_OBJECT_TYPE_FENCE";
			case VK_OBJECT_TYPE_IMAGE_VIEW: return "VK_OBJECT_TYPE_IMAGE_VIEW";
			case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV: return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV";
			case VK_OBJECT_TYPE_INSTANCE: return "VK_OBJECT_TYPE_INSTANCE";
			case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL: return "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL";
			case VK_OBJECT_TYPE_QUEUE: return "VK_OBJECT_TYPE_QUEUE";
			case VK_OBJECT_TYPE_SAMPLER: return "VK_OBJECT_TYPE_SAMPLER";
			case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION: return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
			case VK_OBJECT_TYPE_SEMAPHORE: return "VK_OBJECT_TYPE_SEMAPHORE";
			case VK_OBJECT_TYPE_SHADER_MODULE: return "VK_OBJECT_TYPE_SHADER_MODULE";
			case VK_OBJECT_TYPE_SURFACE_KHR: return "VK_OBJECT_TYPE_SURFACE_KHR";
			case VK_OBJECT_TYPE_SWAPCHAIN_KHR: return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
			case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT: return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
			case VK_OBJECT_TYPE_UNKNOWN: return "VK_OBJECT_TYPE_UNKNOWN";
			case VK_OBJECT_TYPE_MAX_ENUM: return "VK_OBJECT_TYPE_MAX_ENUM";
			}
			LOG_ASSERT(false,"");
			return "";
		}

		inline VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, bool vsync)
		{
			VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			if (!vsync)
			{
				for (size_t i = 0; i < availablePresentModes.size(); i++)
				{
					if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
					{
						swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
						break;
					}
					if (availablePresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
					{
						swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
					}
				}
			}
			return swapchainPresentMode;
		}

		inline std::string GetVulkanDeviceTypeName(VkPhysicalDeviceType type)
		{
			switch (type)
			{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				return "Other";
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				return "Integrated GPU";
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				return "Discrete GPU";
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				return "Virtual GPU";
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				return "CPU";
			default:
				return "UNKNOWN";
			}
		}

		static std::string VulkanVendorIDToString(uint32_t vendorID)
		{
			switch (vendorID)
			{
			case 0x10DE: return "NVIDIA";
			case 0x1002: return "AMD";
			case 0x8086: return "INTEL";
			case 0x13B5: return "ARM";
			}
			return "Unknown";
		}

		namespace Init
		{
			inline VkCommandBufferAllocateInfo CommandBufferAllocateInfo(
				VkCommandPool command_pool,
				VkCommandBufferLevel level,
				uint32_t buffer_count)
			{
				VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
				commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				commandBufferAllocateInfo.commandPool = command_pool;
				commandBufferAllocateInfo.level = level;
				commandBufferAllocateInfo.commandBufferCount = buffer_count;
				return commandBufferAllocateInfo;
			}

			inline VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags = 0)
			{
				VkFenceCreateInfo fence_create_info{};
				fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fence_create_info.flags = flags;
				return fence_create_info;
			}

			inline auto RenderPassCreateInfo()
			{
				VkRenderPassCreateInfo renderPassCreateInfo{};
				renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				return renderPassCreateInfo;
			}
		}
	}

	namespace Num
	{
		template <typename T>
		constexpr T Max()
		{
			return std::numeric_limits<T>::max();
		}
	}
}
