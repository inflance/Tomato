#include "VulkanImGuiLayer.hpp"

#include <imgui.h>
#include "ImGuizmo.h"
#include "VulkanContext.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "Tomato/Core/Engine.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "VulkanSwapChain.hpp"
#include "VulkanCommandExecutor.hpp"
#include "Tomato/Renderer/RendererConfig.hpp"


namespace Tomato
{
	static vk::raii::CommandBuffers s_command_buffers = nullptr;
	static vk::raii::RenderPass s_render_pass = nullptr;


	void VulkanImGuiLayer::OnCreate()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoDecoration = false;
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//SetDarkThemeColors();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);


		const auto& vulkanContext = VulkanContext::Get();

		//set up descriptor poopl
		{
			const auto& device = VulkanContext::Get().device;
			constexpr vk::DescriptorPoolSize pool_sizes[] = {
				{vk::DescriptorType::eSampler, 1000},
				{vk::DescriptorType::eCombinedImageSampler, 1000},
				{vk::DescriptorType::eSampledImage, 1000},
				{vk::DescriptorType::eStorageImage, 1000},
				{vk::DescriptorType::eUniformTexelBuffer, 1000},
				{vk::DescriptorType::eStorageTexelBuffer, 1000},
				{vk::DescriptorType::eUniformBuffer, 1000},
				{vk::DescriptorType::eStorageBuffer, 1000},
				{vk::DescriptorType::eUniformBufferDynamic, 1000},
				{vk::DescriptorType::eStorageBufferDynamic, 1000},
				{vk::DescriptorType::eInputAttachment, 1000}
			};

			vk::DescriptorPoolCreateInfo pool_info{};
			pool_info.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			         .setMaxSets(100 * IM_ARRAYSIZE(pool_sizes))
			         .setPPoolSizes(pool_sizes)
			         .setPoolSizeCount(static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes)));

			descPool = vk::raii::DescriptorPool(device, pool_info);
		}
		const Engine& app = Engine::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = *vulkanContext.instance;
		init_info.PhysicalDevice = *vulkanContext.physicalDevice;
		init_info.Device = *vulkanContext.device;
		init_info.QueueFamily = vulkanContext.queueFamily.Graphics.value();
		init_info.Queue = *vulkanContext.graphicsQueue;
		init_info.PipelineCache = *vulkanContext.pipelineCache;
		init_info.DescriptorPool = *descPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = 2;
		auto& swapChain = vulkanContext.swapChain;
		init_info.ImageCount = swapChain->GetImageCount();
		init_info.CheckVkResultFn = [](VkResult res)
		{
			LOG_ASSERT(res == VK_SUCCESS, "")
		};
		ImGui_ImplVulkan_Init(&init_info, *vulkanContext.renderPass);

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);
		//Upload Fonts
		{
			CommandExecutor::Get().ImmediateExecute(vulkanContext.graphicsQueue,
			                                        [](const vk::raii::CommandBuffer& cmdBuf)mutable
			                                        {
				                                        ImGui_ImplVulkan_CreateFontsTexture(*cmdBuf);
			                                        });
			ImGui_ImplVulkan_DestroyFontUploadObjects();

			const auto FrameInFlight = RendererConfig::MaxFrameInFlight;

			s_command_buffers = CommandExecutor::CreateCommandBuffers(FrameInFlight);
		}

	}


	void VulkanImGuiLayer::OnDestroy()
	{
		const auto& device = VulkanContext::Get().device;

		device.waitIdle();
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void VulkanImGuiLayer::OnEvent(Event& event)
	{
	}

	void VulkanImGuiLayer::OnImGuiRenderer()
	{
		
	}

	void VulkanImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void VulkanImGuiLayer::End()
	{
		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, *VulkanContext::Get().GetCurrentCommandBuffer());

		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void VulkanImGuiLayer::SetDarkModeColor()
	{
	}
}
