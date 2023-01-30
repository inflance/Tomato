#pragma once

#include <vulkan/vulkan.h>

#include "VulkanUtils.h"
#include "Tomato/Core/Macro.h"
#include "Tomato/Core/PublicSingleton.h"

namespace Tomato::Debug
{
	constexpr std::string_view VkDebugUtilsMessageType(const VkDebugUtilsMessageTypeFlagsEXT type)
	{
		switch (type)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:		return "General";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:	return "Validation";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:	return "Performance";
		default:												return "Unknown";
		}
	}

	constexpr std::string_view VkDebugUtilsMessageSeverity(const VkDebugUtilsMessageSeverityFlagBitsEXT severity)
	{
		switch (severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		return "error";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	return "warning";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		return "info";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:	return "verbose";
		default:												return "unknown";
		}
	}



	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugUtilsMessengerCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, const VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		(void)pUserData; //Unused argument

		const bool performanceWarnings = false;
		if (!performanceWarnings)
		{
			if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
				return VK_FALSE;
		}

		std::string labels, objects;
		if (pCallbackData->cmdBufLabelCount)
		{
			labels = fmt::format("\tLabels({}): \n", pCallbackData->cmdBufLabelCount);
			for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; ++i)
			{
				const auto& label = pCallbackData->pCmdBufLabels[i];
				const std::string colorStr = fmt::format("[ {}, {}, {}, {} ]", label.color[0], label.color[1], label.color[2], label.color[3]);
				labels.append(std::format("\t\t- Command Buffer Label[{0}]: name: {1}, color: {2}\n", i, label.pLabelName ? label.pLabelName : "NULL", colorStr));
			}
		}

		if (pCallbackData->objectCount)
		{
			objects = fmt::format("\tObjects({}): \n", pCallbackData->objectCount);
			for (uint32_t i = 0; i < pCallbackData->objectCount; ++i)
			{
				const auto& object = pCallbackData->pObjects[i];
				objects.append(std::format("\t\t- Object[{0}] name: {1}, type: {2}, handle: {3:#x}\n", i, object.pObjectName ? object.pObjectName : "NULL", Utils::VkObjectTypeToString(object.objectType), object.objectHandle));
			}
		}

		LOG_WARN("{0} {1} message: \n\t{2}\n {3} {4}", VkDebugUtilsMessageType(messageType), VkDebugUtilsMessageSeverity(messageSeverity), pCallbackData->pMessage, labels, objects);

		return VK_FALSE;
	}
}
