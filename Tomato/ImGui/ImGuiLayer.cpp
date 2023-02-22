#include "ImGuiLayer.h"

#include <imgui.h>
#include <glfw/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "Tomato/Core/Engine.h"

namespace Tomato {

	void ImGuiLayer::OnCreate()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//ImGui::SetCurrentContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		float fontSize = 18.0f;// *2.0f;
		//io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF( "C:/Users/liyun/source/repos/Tomato/Precompile/Assets/Fonts/Myriad-Pro_31655.ttf", fontSize);
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		SetDarkModeColor();
		//ImGui::StyleColorsClassic();

		
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		TomatoEngine& app = TomatoEngine::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiLayer::OnDestroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (m_block)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.handle |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.handle |= event.IsInCategory(EventCategoryKey) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnImGuiRenderer()
	{
		
	}


	void ImGuiLayer::SetDarkModeColor()
	{
		auto& colors = ImGui::GetStyle().Colors;
		
		colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.0f);

		colors[ImGuiCol_Header] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);	

		colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
		
		colors[ImGuiCol_CheckMark] = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.99f, 0.99f, 0.99f, 1.0f);

		colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.17f, 0.17f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.17f, 0.17f, 0.17f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		
		colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.96f, 0.96f, 0.96f, 1.0f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

		auto& padding = ImGui::GetStyle().FramePadding;
		padding = ImVec2(20.0f, 8.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

		auto& position = ImGui::GetStyle().WindowMenuButtonPosition;
		position += 1;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)TomatoEngine::GetInstance().GetWindow().GetWidth(), (float)TomatoEngine::GetInstance().GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}