#pragma once

#include <deque>

#include "Tomato/Renderer/Renderer.h"
#include "Tomato/Core/Window.h"
#include <vulkan/vulkan.h>

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"
#include "VulkanUtils.h"
#include "Tomato/Renderer/GraphicsContext.h"

namespace Tomato
{
	class VulkanSwapChain;
	class VulkanCommandBuffer;

	namespace Config
	{
		static constexpr bool s_standard_validation_layer = false;
		static constexpr bool s_assistance_layer = false;
		static constexpr bool s_enable_validation_layers = true;
	}


	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(Window* window);
		~VulkanContext() override;

		void CreatePipelineCache();
		void Init() override;
		void WaitIdle() override;
		void Destroy();

		[[nodiscard]] std::shared_ptr<VulkanDevice> GetDevice() const { return m_device; }
		static VkInstance GetInstance() { return GetContext()->m_instance; }

		static std::shared_ptr<VulkanContext> GetContext(){return As<VulkanContext>(TomatoEngine::GetContext());}
		static Ref<VulkanDevice> GetCurrentDevice() { return GetContext()->GetDevice(); }
		static std::shared_ptr<VulkanSwapChain> GetCurrentSwapChain() { return GetContext()->GetSwapChain(); }

		std::shared_ptr<VulkanSwapChain> GetSwapChain() { return m_swap_chain; }
		static CommandQueue& GetQueue(uint32_t count) { assert(count < 3); return m_queue[count]; }

		bool CheckValidationLayerSupport(std::vector<VkLayerProperties>& layers, std::vector<const char*>& validation_layers);
		
	private:
		void CreateInstance();
		void SetUpDebug();

	private:
		static constexpr uint32_t maxsize = 3;
		VkInstance m_instance;
		
		Window* m_window_handle;
		std::shared_ptr<VulkanPhysicalDevice> m_physical_device;
		std::shared_ptr<VulkanDevice> m_device = nullptr;
		std::shared_ptr<VulkanSwapChain> m_swap_chain = nullptr;

		std::vector<const char*>		m_layer_names;
		std::vector<const char*>          m_extension_names;
		std::vector<VkLayerProperties>     m_layer_propertieses;
		std::vector<VkExtensionProperties> m_extension_propertieses;
		VkDebugUtilsMessengerEXT m_debug_utils_messenger;

		inline static CommandQueue m_queue[maxsize];
		std::shared_ptr<VulkanFence> m_fence;
		VkPipelineCache m_pipeline_cache = nullptr;
	};
}
