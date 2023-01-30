#include "VulkanContext.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "Tomato/Core/Macro.h"
#include "VulkanUtils.h"
#include "VulkanDebug.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapChain.h"

constexpr const char* VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME = "VK_LAYER_LUNARG_standard_validation";
constexpr const char* VK_LAYER_LUNARG_ASSISTENT_LAYER_NAME = "VK_LAYER_LUNARG_assistant_layer";
constexpr const char* VK_LAYER_LUNARG_VALIDATION_NAME = "VK_LAYER_KHRONOS_validation";

namespace Tomato
{

	inline auto GetRequiredLayers()
	{
		std::vector<const char*> layers;
		if constexpr (Config::s_standard_validation_layer)
			layers.emplace_back(VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME);

		if constexpr (Config::s_assistance_layer)
			layers.emplace_back(VK_LAYER_LUNARG_ASSISTENT_LAYER_NAME);

		if constexpr (Config::s_enable_validation_layers)
		{
			layers.emplace_back(VK_LAYER_LUNARG_VALIDATION_NAME);
		}
		return layers;
	}

	inline auto getRequiredExtensions()
	{
		std::vector<const char*> extensions;

		if constexpr (Config::s_standard_validation_layer)
		{
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extensions.emplace_back("VK_KHR_surface");
#if defined(TRACY_ENABLE) && defined(PLATFORM_WINDOWS)
		//extensions.emplace_back("VK_EXT_calibrated_timestamps");
#endif

#if defined(_WIN32)
		extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		extensions.emplace_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
		extensions.emplace_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		extensions.emplace_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
		extensions.emplace_back("VK_EXT_metal_surface");
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		extensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_METAL_EXT)
		extensions.emplace_back("VK_EXT_metal_surface");
#endif
		return extensions;
	}

	inline bool CheckExtensionSupport(std::vector<VkExtensionProperties>& properties, std::vector<const char*>& extensions)
	{
		uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		properties.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, properties.data());

		bool extensionSupported = true;

		for (int i = 0; i < extensions.size(); i++)
		{
			const char* extensionName = extensions[i];
			bool        layerFound = false;

			for (const auto& layerProperties : properties)
			{
				if (strcmp(extensionName, layerProperties.extensionName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				extensions.erase(extensions.begin() + i);
				extensionSupported = false;
				LOG_WARN("Extension not supported {0}", extensionName);
			}
		}
		return extensionSupported;
	}

	const std::string s_application_name = "Tomato Editor";
	const std::string s_engine_name = "Tomato Engine";
	constexpr uint32_t s_api_version = VK_API_VERSION_1_2;


	VulkanContext::VulkanContext(Window* window)
		: m_window_handle(window)
	{
	}

	VulkanContext::~VulkanContext()
	{
		Destroy();
	}

	void VulkanContext::Init()
	{
		LOG_INFO("Vulkan: VulkanContent Init");
		CreateInstance();
		SetUpDebug();

		m_physical_device = VulkanPhysicalDevice::Select();
		VkPhysicalDeviceFeatures enabledFeatures{};
		enabledFeatures.samplerAnisotropy = true;
		enabledFeatures.wideLines = true;
		enabledFeatures.fillModeNonSolid = true;
		enabledFeatures.independentBlend = true;
		enabledFeatures.pipelineStatisticsQuery = true;

		m_device = std::make_shared<VulkanDevice>(m_physical_device, enabledFeatures);

		CreatePipelineCache();
		m_swap_chain = std::make_shared<VulkanSwapChain>(m_window_handle->GetWidth(), m_window_handle->GetHeight());
		m_swap_chain->Init(m_instance, m_device);
		m_swap_chain->InitSurface(static_cast<GLFWwindow*>(m_window_handle->GetNativeWindow()));
	}

	void VulkanContext::Destroy()
	{
		LOG_INFO("Vulkan: VulkanContent Destroy!");
		//vkDestroyPipelineCache(m_device->GetVulkanDevice(), m_pipeline_cache, nullptr);
		auto swap_chain = As<VulkanSwapChain>(m_swap_chain);
		swap_chain->Destroy();

		if (Config::s_enable_validation_layers)
		{
			
		}
		m_device->Destroy();
		vkDestroyInstance(m_instance, nullptr);
	}

	void VulkanContext::CreateInstance()
	{
		LOG_INFO("VulkanContext::Create Instance");

		LOG_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hazel";
		appInfo.pEngineName = "Hazel";
		appInfo.apiVersion = VK_API_VERSION_1_2;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Extensions and Validation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Very little performance hit, can be used in Release.
		if (Config::s_enable_validation_layers)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
		VkValidationFeaturesEXT features = {};
		features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		features.enabledValidationFeatureCount = 1;
		features.pEnabledValidationFeatures = enables;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr; // &features;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		// TODO: Extract all validation into separate class
		if (Config::s_enable_validation_layers)
		{
			const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
			// Check if this layer is available at instance level
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			bool validationLayerPresent = false;
			LOG_TRACE("Vulkan Instance Layers:");
			for (const VkLayerProperties& layer : instanceLayerProperties)
			{
				LOG_TRACE("  {0}", layer.layerName);
				if (strcmp(layer.layerName, validationLayerName) == 0)
				{
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent)
			{
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else
			{
				LogSystem::LogError("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Instance and Surface Creation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance), "");
	}

	bool VulkanContext::CheckValidationLayerSupport(std::vector<VkLayerProperties>& layers, std::vector<const char*>& validation_layers)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		layers.resize(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : layers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
			{
				return false;
			}
		}
		return true;
	}

	void VulkanContext::WaitIdle()
	{
		vkDeviceWaitIdle(m_device->GetVulkanDevice());
	}

	void VulkanContext::SetUpDebug()
	{
		Utils::VulkanLoadDebugUtilsExtensions(m_instance);
		if(Config::s_enable_validation_layers)
		{
			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
			LOG_ASSERT(vkCreateDebugUtilsMessengerEXT != NULL, "");
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
			debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugUtilsCreateInfo.pfnUserCallback = Debug::VulkanDebugUtilsMessengerCallback;
			debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT*/;

			VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(m_instance, &debugUtilsCreateInfo, nullptr, &m_debug_utils_messenger),"");
		}
	}

	void VulkanContext::CreatePipelineCache()
	{
		// Pipeline Cache
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(m_device->GetVulkanDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipeline_cache), "");
	}

}