#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Tomato/Core/Macro.h"
#include "Tomato/Core/Window.h"

namespace Tomato
{
	class VulkanCommandPool;

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> Graphics;
		std::optional<uint32_t> Compute;
		std::optional<uint32_t> Transfer;

		constexpr bool IsComplete() const
		{
			return Graphics.has_value() && Compute.has_value() && Transfer.has_value();
		}
	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		void Init();
		void Destroy() const;

		inline bool IsExtensionSupported(const std::string& extensionName) const
		{
			return m_SupportedExtensions.find(extensionName) != m_SupportedExtensions.end();
		}
		static auto Select() { return std::make_shared<VulkanPhysicalDevice>(); }
		[[nodiscard]] auto GetVkPhysicalDevice() const { return m_PhysicalDevice; }
		[[nodiscard]] auto& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }
		[[nodiscard]] auto& GetProps() const { return m_Properties; }
		[[nodiscard]] auto& GetFeatures() const { return m_Features; }
		[[nodiscard]] auto& GetDepthFormat() const { return m_DepthFormat; }
		const VkPhysicalDeviceLimits& GetLimits() const { return m_Properties.limits; }

		uint32_t GetMemoryType(uint32_t type_bits, VkMemoryPropertyFlags properties) const;

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device);
		VkFormat FindDepthFormat() const;
		QueueFamilyIndices FindQueueFamilies(int queueFlags);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	private:
		QueueFamilyIndices m_QueueFamilyIndices;

		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::unordered_set<std::string> m_SupportedExtensions;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;

		friend class VulkanDevice;
	};

	// Represents a logical device
	class VulkanDevice
	{
	public:
		VulkanDevice(const Ref<VulkanPhysicalDevice>& physicalDevice, VkPhysicalDeviceFeatures enabledFeatures);
		~VulkanDevice();

		void Destroy();

		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetComputeQueue() { return m_ComputeQueue; }

		VkCommandBuffer GetCommandBuffer(bool begin, bool compute = false);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue);

		VkCommandBuffer CreateSecondaryCommandBuffer(const char* debugName) const;

		const Ref<VulkanPhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetVulkanDevice() const { return m_LogicalDevice; }
	private:
		VkDevice m_LogicalDevice = nullptr;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		VkPhysicalDeviceFeatures m_EnabledFeatures {};
		VkCommandPool m_CommandPool = nullptr, m_ComputeCommandPool = nullptr;

		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;

		bool m_EnableDebugMarkers = false;
	};
}
