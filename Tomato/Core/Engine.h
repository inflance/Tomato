#pragma once

#include <chrono>

#include "Tomato/Core/Layer.h"
#include "Tomato/Core/Window.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Core/LayerStack.h"
#include "Tomato/ImGui/ImGuiLayer.h"
#include "Tomato/Events/ApplicationEvent.h"

namespace Tomato {

	class TomatoEngine
	{
	public:
		void StartUp();
		void ShutDown();

		static TomatoEngine& GetInstance() {
			static TomatoEngine instance;
			return instance;
		}
		void Run();
		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* over_layer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* over_layer);

		[[nodiscard]] uint32_t GetFPS() const { return m_fps; }
		[[nodiscard]] Window& GetWindow() const { return *m_window; }
		[[nodiscard]] ImGuiLayer* GetImGuiLayer() const { return m_imgui_layer; }

		void SetVSync(bool is_vsync) const { m_window->SetVSync(is_vsync); };
	private:
		TomatoEngine() = default;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		float CalculateDeltaTime();
		void CalculateFPS(float delta_time);

	protected:
		float m_last_time = 0.0f;
		bool m_minimized = false;

		//for caculate delta time
		const float k_fps_alpha = 1.0f / 100.0f;
		std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };
		uint32_t m_frame_count{ 0 };
		uint32_t m_fps{ 0 };
		float m_average_duration{ 0.0f };
	private:
		bool m_running = true;

		Ref<Window> m_window;
		LayerStack m_layer_stack;
		ImGuiLayer* m_imgui_layer;
	};
}