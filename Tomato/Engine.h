#pragma once

#include <iostream>

#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"

//Event
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
//

#include "ImGui/ImGuiLayer.h"

#include <chrono>

namespace Tomato {

	class ImGuiLayer;

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
		TomatoEngine();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnwindowResize(WindowResizeEvent& e);
		
		float CalculateDeltaTime();
		void CalculateFPS(float deltaTime);



	private:
		std::shared_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layer_stack;
		ImGuiLayer* m_imgui_layer;

	protected:
		TimeSpan m_lastTime = 0.0f;
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