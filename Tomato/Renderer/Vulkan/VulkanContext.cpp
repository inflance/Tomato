#include "VulkanContext.h"

#include "Tomato/Core/Macro.h"
#include "VulkanUtils.h"
#include "VulkanDebug.h"
#include <ThirdParty/Vulkan-Hpp/RAII_Samples/utils/utils.hpp>
#ifdef TMT_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "VulkanShader.h"
#include "VulkanConfig.h"
#include "VulkanSwapChain.h"
#include "Tomato/Renderer/RendererConfig.h"

constexpr const char* VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME = "VK_LAYER_LUNARG_standard_validation";
constexpr const char* VK_LAYER_LUNARG_ASSISTENT_LAYER_NAME = "VK_LAYER_LUNARG_assistant_layer";
constexpr const char* VK_LAYER_LUNARG_VALIDATION_NAME = "VK_LAYER_KHRONOS_validation";

namespace Tomato
{
	inline std::vector<std::string> GetInstanceExtensions()
	{
		std::vector<std::string> extensions;
		extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
		extensions.emplace_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_IOS_MVK )
		extensions.emplace_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
		extensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_MIR_KHR )
		extensions.emplace_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_VI_NN )
		extensions.emplace_back(VK_NN_VI_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
		extensions.emplace_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_WIN32_KHR )
		extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XCB_KHR )
		extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
		extensions.emplace_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
		extensions.emplace_back(VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME);
#endif
		return extensions;
	}

	static void PrintStr(std::vector<const char*>& strings, std::string_view name = "")
	{
		for (auto& str : strings)
		{
			LOG_INFO("{0} {1}", name, str);
		}
	}

	inline auto GetRequiredLayers()
	{
		std::vector<std::string> layers;
		if constexpr (VKConfig::StandardValidationLayer)
			layers.emplace_back(VK_LAYER_LUNARG_STANDARD_VALIDATION_NAME);

		if constexpr (VKConfig::AssistanceLayer)
			layers.emplace_back(VK_LAYER_LUNARG_ASSISTENT_LAYER_NAME);

		if constexpr (VKConfig::EnableValidationLayers)
		{
			layers.emplace_back(VK_LAYER_LUNARG_VALIDATION_NAME);
		}
		return layers;
	}

	std::vector<const char*> GetDeviceExtensions()
	{
		return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	}


	VulkanContext::VulkanContext(Window* window)
		: m_window_handle(window), m_width(window->GetWidth()), m_height(window->GetHeight())
	{
	}

	VulkanContext::~VulkanContext()
	{
		Destroy();
	}

	void VulkanContext::Begin()
	{
		commandBuffers[cur_frame].reset({});

		vk::Result result;

		std::tie(result, imageIndex) = swapChain->swapChain.acquireNextImage(
			vk::su::FenceTimeout, *imageAcquiredSemaphore[cur_frame]);
		if (result == vk::Result::eSuccess)
			LOG_ASSERT(result == vk::Result::eSuccess, vk::to_string(result));
		ASSERT(imageIndex < swapChain->images.size());
	}

	void VulkanContext::Init()
	{
		LOG_TRACE("Vulkan content init");

		instanceExt = GetInstanceExtensions();
		requiredLay = GetRequiredLayers();
		// create an Instance
		{
			LOG_TRACE("Vulkan instance init");
			vk::ApplicationInfo applicationInfo;
			applicationInfo.setPApplicationName(VKConfig::ApplicationName.c_str())
			               .setEngineVersion({})
			               .setApiVersion(VKConfig::ApiVersion)
			               .setApplicationVersion({})
			               .setPEngineName(VKConfig::EngineName.c_str());
			enabledLayers = Utils::GetEnableLayers(
				requiredLay, context.enumerateInstanceLayerProperties());
			enabledExtensions = Utils::gatherExtensions(
				instanceExt, context.enumerateInstanceExtensionProperties());
			PrintStr(enabledLayers, "Enabled layers:");
			PrintStr(enabledExtensions, "Enabled Extensions: ");

			vk::InstanceCreateInfo create_info;
			create_info.setFlags({})
			           .setPEnabledLayerNames(enabledLayers)
			           .setEnabledLayerCount(enabledLayers.size())
			           .setPEnabledExtensionNames(enabledExtensions)
			           .setEnabledExtensionCount(enabledExtensions.size())
			           .setPApplicationInfo(&applicationInfo);
			instance = vk::raii::Instance(context, create_info);
		}


#if !defined( NDEBUG )
		//set up debug utils messenger
		{
			LOG_TRACE("Vulkan debug utils messenger init");
			vk::DebugUtilsMessengerCreateInfoEXT create_info;
			create_info.setFlags({})
			           .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				           vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			           .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
			           .setPfnUserCallback(&Debug::VulkanDebugUtilsMessengerCallback);
			debugUtilsMessenger = vk::raii::DebugUtilsMessengerEXT(instance, create_info);
		}

#endif
		// enumerate the physical devices
		{
			LOG_TRACE("Vulkan enumerate the physical devices");
			const auto physical_devices = vk::raii::PhysicalDevices(instance);

			LOG_TRACE("Vulkan select the physical device");
			physicalDevice = physical_devices.front();
			auto device_ex_props = physicalDevice.enumerateDeviceExtensionProperties();

			for (auto& prop : device_ex_props)
			{
				LOG_INFO("Device extension {}", prop.extensionName);
			}
		}

		// find queue family properties
		queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		{
			auto physicalDeviceProps = physicalDevice.getProperties();

			LOG_INFO("Vulkan api version {}", VK_VERSION_MAJOR(physicalDeviceProps.apiVersion),
			         VK_VERSION_MINOR(physicalDeviceProps.apiVersion),
			         VK_VERSION_PATCH(physicalDeviceProps.apiVersion));
			LOG_INFO("Device name {}", physicalDeviceProps.deviceName);
			LOG_INFO("Device device ID {}", physicalDeviceProps.deviceID);
			LOG_INFO("Device device type {}", vk::to_string(physicalDeviceProps.deviceType));
			LOG_INFO("Device driver version {}", physicalDeviceProps.driverVersion);
		}


		CreateSwapChain();
	}

	void VulkanContext::CreateSwapChain()
	{
		LOG_TRACE("Vulkan context create swap chain");
		swapChain = As<VulkanSwapChain>(SwapChain::Create(m_window_handle, m_width, m_height));
		//surfaceData = Utils::SurfaceData(instance, m_window_handle, vk::Extent2D(m_width, m_height));

		queueFamily = FindQueueFamily();

		ASSERT(queueFamily.IsComplete());

		{
			float queuePriority = 0.0f;
			vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily.Graphics.value(),
			                                                1,
			                                                &queuePriority);
			std::vector<const char*> ext_names(GetDeviceExtensions());
			vk::DeviceCreateInfo deviceCreateInfo;
			deviceCreateInfo.setFlags(vk::DeviceCreateFlags())
			                .setQueueCreateInfos(deviceQueueCreateInfo)
			                .setPEnabledLayerNames(enabledLayers)
			                .setPEnabledExtensionNames(ext_names);

			device = vk::raii::Device(physicalDevice, deviceCreateInfo);
		}

		{
			// create a CommandPool to allocate a CommandBuffer
			const vk::CommandPoolCreateInfo commandPoolCreateInfo({vk::CommandPoolCreateFlagBits::eResetCommandBuffer},
			                                                      queueFamily.Graphics.value());
			commandPool = vk::raii::CommandPool(device, commandPoolCreateInfo);

			// allocate a CommandBuffer from the CommandPool
			const vk::CommandBufferAllocateInfo
				commandBufferAllocateInfo(*commandPool, vk::CommandBufferLevel::ePrimary,
				                          RendererConfig::MaxFrameInFlight);
			commandBuffers = vk::raii::CommandBuffers(device, commandBufferAllocateInfo);
		}

		{
			graphicsQueue = vk::raii::Queue(device, queueFamily.Graphics.value(), 0);
			presentQueue = vk::raii::Queue(device, queueFamily.Present.value(), 0);
		}

		swapChain->InitSwapChain(m_width, m_height);
		m_depth_buffer_data = Utils::DepthBufferData(physicalDevice, device, vk::Format::eD16Unorm, swapChain->extent);

		const vk::Format colorFormat = vk::su::pickSurfaceFormat(
				physicalDevice.getSurfaceFormatsKHR(*swapChain->surface)).
			format;
		renderPass = vk::raii::su::makeRenderPass(device, colorFormat, m_depth_buffer_data.format);

		frameBuffers =
			vk::raii::su::makeFramebuffers(device, renderPass, swapChain->imageViews, &m_depth_buffer_data.imageView,
			                               swapChain->extent);

		pipelineCache = vk::raii::PipelineCache(device, vk::PipelineCacheCreateInfo());

		drawFences.reserve(swapChain->imageCount);
		for (uint32_t i = 0; i < swapChain->imageCount; i++)
			drawFences.emplace_back(device, vk::FenceCreateInfo());
		imageAcquiredSemaphore.reserve(swapChain->imageCount);
		for (uint32_t i = 0; i < swapChain->imageCount; i++)
			imageAcquiredSemaphore.emplace_back(device, vk::SemaphoreCreateInfo());
	}

	void VulkanContext::Destroy()
	{
		LOG_INFO("Vulkan context destroy!");
	}

	void VulkanContext::Present()
	{
		device.resetFences(*drawFences[cur_frame]);

		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		vk::SubmitInfo submitInfo(*imageAcquiredSemaphore[cur_frame], waitDestinationStageMask,
		                          *commandBuffers[cur_frame]);
		graphicsQueue.submit(submitInfo, *drawFences[cur_frame]);

		while (vk::Result::eTimeout == device.waitForFences({*drawFences[cur_frame]}, VK_TRUE, vk::su::FenceTimeout));

		const vk::PresentInfoKHR presentInfoKHR(nullptr, *swapChain->swapChain, imageIndex);
		const vk::Result result = presentQueue.presentKHR(presentInfoKHR);
		switch (result)
		{
		case vk::Result::eSuccess: break;
		//
		case vk::Result::eSuboptimalKHR: std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
			break;
		default: assert(false); // an unexpected result is returned !
		}

		cur_frame = (cur_frame + 1) % RendererConfig::MaxFrameInFlight;
		device.waitIdle();
	}

	void VulkanContext::SwapChainResize(uint32_t width, uint32_t height)
	{
		swapChain->InitSwapChain(width, height);
		m_depth_buffer_data = Utils::DepthBufferData(physicalDevice, device, vk::Format::eD16Unorm, swapChain->extent);
		const vk::Format colorFormat = vk::su::pickSurfaceFormat(
				physicalDevice.getSurfaceFormatsKHR(*swapChain->surface)).
			format;

		renderPass = vk::raii::su::makeRenderPass(device, colorFormat, m_depth_buffer_data.format);
		frameBuffers =
			vk::raii::su::makeFramebuffers(device, renderPass, swapChain->imageViews, &m_depth_buffer_data.imageView,
			                               swapChain->extent);
	}

	void VulkanContext::OnResize(uint32_t width, uint32_t height)
	{
		device.waitIdle();
		m_width = width;
		m_height = height;

		SwapChainResize(m_width, m_height);

		device.waitIdle();
	}

	QueueFamily VulkanContext::FindQueueFamily()
	{
		auto& surface = swapChain->surface;
		ASSERT(queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max());

		const auto graphicsQueueFamilyProperty =
			std::find_if(queueFamilyProperties.begin(),
			             queueFamilyProperties.end(),
			             [](const vk::QueueFamilyProperties& qfp)
			             {
				             return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
			             });
		ASSERT(graphicsQueueFamilyProperty != queueFamilyProperties.end());
		uint32_t graphicsQueueFamilyIndex = static_cast<uint32_t>(std::distance(
			queueFamilyProperties.begin(), graphicsQueueFamilyProperty));

		if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, *surface))
		{
			return QueueFamily({graphicsQueueFamilyIndex}, {graphicsQueueFamilyIndex});
			// the first graphicsQueueFamilyIndex does also support presents
		}

		// the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
		// graphics and present
		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
				physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface))
			{
				return QueueFamily(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
			}
		}

		// there's nothing like a single family index that supports both graphics and present -> look for an other
		// family index that supports present
		for (size_t i = 0; i < queueFamilyProperties.size(); i++)
		{
			if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface))
			{
				return QueueFamily(graphicsQueueFamilyIndex, static_cast<uint32_t>(i));
			}
		}

		throw std::runtime_error("Could not find queues for both graphics or present -> terminating");
	}
}
