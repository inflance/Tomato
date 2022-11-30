#pragma once

#include <chrono>
//Event

#include "Tomato/Core/Window.h"
#include "Tomato/Core/Layer.h"
#include "Tomato/Core/LayerStack.h"
#include "Tomato/ImGui/ImGuiLayer.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/KeyEvent.h"
#include "Tomato/Events/MouseEvent.h"

namespace Tomato {

	class TomatoEngine 
	{
	public:
		
		void StartUp();
		void ShutDown();

		inline static TomatoEngine& GetInstance() {
			static TomatoEngine instance;
			return instance;
		}
		void Run();
		void OnEvent(Event& e);

		void Close();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLay);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overLay);

		Window& GetWindow() const { return *m_window; }

		uint32_t GetFPS() const { return m_fps; }		
		
		ImGuiLayer* GetImGuiLayer() { return m_imgui_layer; }

	private:
		TomatoEngine() = default;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnwindowResize(WindowResizeEvent& e);
		
		//计算Delta时间
		float CalculateDeltaTime();
		//计算FPS
		void CalculateFPS(float deltaTime);

	private:
		bool m_running = true;

		std::shared_ptr<Window> m_window;
		
		LayerStack m_layer_stack;
		ImGuiLayer* m_imgui_layer;

	protected:
		float m_lastTime = 0.0f;
		bool m_minimized = false;

		//for caculate delta time
		std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };
		uint32_t m_frame_count{ 0 };
		uint32_t m_fps{ 0 };
		float m_average_duration{ 0.0f };

		//for caculate fps
		const float k_fps_alpha = 1.0f / 100.0f;
	};

}