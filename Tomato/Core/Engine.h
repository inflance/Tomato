#pragma once

#include <chrono>

#include "IDGener.h"
#include "Singleton.h"
#include "Tomato/Core/Layer.h"
#include "Tomato/Core/Window.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Core/LayerStack.h"
#include "Tomato/ImGui/ImGuiLayer.hpp"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/ECS/World/World.hpp"

namespace Tomato
{

	class Engine : public Singleton<Engine>
	{
	public:
		void StartUp();
		void ShutDown();

		void Run();
		void OnEvent(Event& e);
		void Close();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* over_layer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* over_layer);

		static auto GetContext() { return Get().m_context; }
		static auto GetWorld() { return Get().m_world; }
		static auto GenID() { return Get().m_generator.GenID(); }

		[[nodiscard]] uint32_t GetFPS() const { return m_fps; }

		[[nodiscard]] Window& GetWindow() const { return *m_window; }

		[[nodiscard]] ImGuiLayer* GetImGuiLayer() const { return m_imgui_layer; }

		void SetVSync(bool is_v_sync) const { m_window->SetVSync(is_v_sync); }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		float CalculateDeltaTime();
		void CalculateFPS(float delta_time);

	protected:
		float m_last_time = 0.0f;
		bool m_minimized = false;

		//for caculate delta time
		const float k_fps_alpha = 1.0f / 100.0f;
		std::chrono::steady_clock::time_point m_last_tick_time_point{std::chrono::steady_clock::now()};
		uint32_t m_frame_count{0};
		uint32_t m_fps{0};
		float m_average_duration{0.0f};

		bool m_running = true;

		Ref<Window> m_window = nullptr;
		Ref<GraphicsContext> m_context = nullptr;
		LayerStack m_layer_stack;
		ImGuiLayer* m_imgui_layer = nullptr;
		DeviceProps m_device_props{};
		std::string m_device_str;
		World m_world;
		IDGenerator m_generator;
	};
}
