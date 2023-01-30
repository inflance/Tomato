#include "Engine.h"
#include "Engine.h"

#include <glm/glm.hpp>

#include "Tomato/Core/Core.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Renderer/Vulkan/VulkanSwapChain.h"
#include "Tomato/Platform/Windows/WindowsWindow.h"
#include "Tomato/Renderer/Renderer.h"

namespace Tomato
{
	void TomatoEngine::StartUp()
	{
		LogSystem::Log(LogType::Info, "Engine StartUp");

		//Window Init
		m_window = Window::Create();
		m_window->SetVSync(true);
		m_window->SetEventCallback(BIND_EVENT_FUNC(TomatoEngine::OnEvent));
		m_context = GraphicsContext::Create(m_window.get());
		m_context->Init();
		const auto props = m_window->GetDeviceProps();
		m_device_props = props;
		m_device_str = std::format("Tomato Engine - {} - ", m_device_props.device_name);
		//Renderer Init
		Renderer::Init();

		//ImGuiLayer Init
		//m_imgui_layer = new ImGuiLayer();
		//PushOverLayer(m_imgui_layer);
	}

	void TomatoEngine::ShutDown()
	{
		LogSystem::Log(LogType::Info, "Engine ShutDown");
	}

	void TomatoEngine::Run()
	{
		LogSystem::Log(LogType::Info, "Engine Run");
		static float total = 0.0f;

		while (m_running)
		{
			const float delta_time = CalculateDeltaTime();
			total += delta_time;
			CalculateFPS(delta_time);
			if (!m_minimized)
			{
				for (Layer* layer : m_layer_stack)
				{
					layer->Tick(delta_time);
				}
			}

		/*	m_imgui_layer->Begin();
			for (Layer* layer : m_layer_stack)
			{
				layer->OnImGuiRenderer();
			}
			m_imgui_layer->End();*/

			if(total > 1.0f)
			{
				m_window->SetWindowTitle(std::format("{1}{0} fps", m_fps, m_device_str));
				total = 0.0f;
			}
			
			m_window->Tick();
		}
	}

	void TomatoEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(TomatoEngine::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(TomatoEngine::OnWindowResize));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.handle)
				break;
		}
	}

	void TomatoEngine::Close()
	{
		m_running = false;
	}

	void TomatoEngine::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
	}

	void TomatoEngine::PushOverLayer(Layer* over_layer)
	{
		m_layer_stack.PushOverLayer(over_layer);
	}

	void TomatoEngine::PopLayer(Layer* layer)
	{
		m_layer_stack.PopLayer(layer);
	}

	void TomatoEngine::PopOverLayer(Layer* over_layer)
	{
		m_layer_stack.PopOverLayer(over_layer);
	}

	bool TomatoEngine::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false;
	}

	bool TomatoEngine::OnWindowResize(WindowResizeEvent& e)
	{
		const uint32_t width = e.GetWidth(), height = e.GetHeight();

		if (width == 0 || height == 0)
		{
			m_minimized = true;
			return false;
		}
		m_minimized = false;
		Renderer::OnWindowResize(0, 0, width, height);
		return false;
	}

	float TomatoEngine::CalculateDeltaTime()
	{
		float delta_time;
		{
			using namespace std::chrono;

			const steady_clock::time_point tick_time_point = steady_clock::now();
			const auto time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
			delta_time = time_span.count();

			m_last_tick_time_point = tick_time_point;
		}
		return delta_time;
	}

	void TomatoEngine::CalculateFPS(float delta_time)
	{
		m_average_duration = m_average_duration * (1 - k_fps_alpha) + delta_time * k_fps_alpha;
		m_fps = static_cast<uint32_t>(1.0f / m_average_duration);
	}
}
