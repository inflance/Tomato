#include "Engine.h"

#include <chrono>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Tomato/Core/Core.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/KeyEvent.h"
#include "Tomato/Events/MouseEvent.h"
#include "Tomato/Renderer/Renderer.h"

namespace Tomato {

	void TomatoEngine::StartUp()
	{
		LogSystem::ConsoleLog("TMT Engine StartUp", LogType::Info);

		//Window Init
		//´°¿Ú³õÊ¼»¯
		m_window = Window::Create();
		m_window->SetVSync(true);
		m_window->SetEventCallback(BIND_EVENT_FUNC(&TomatoEngine::OnEvent));
		
		//Renderer Init
		Renderer::Init();

		//ImGuiLayer Init
		m_imgui_layer = new ImGuiLayer();
		PushOverLayer(m_imgui_layer);	
	}

	void TomatoEngine::ShutDown()
	{
		LogSystem::ConsoleLog("TMT Engine ShutDown", LogType::Info);
	}

	void TomatoEngine::Run()
	{
		LogSystem::ConsoleLog("TMT Engine Running", LogType::Info);

		while (m_running)
		{
			float deltaTime = CalculateDeltaTime();
			CalculateFPS(deltaTime);
			if (!m_minimized) {

				for (Layer* layer : m_layer_stack)
				{
					layer->Tick(deltaTime);
				}
			}

			m_imgui_layer->Begin();
			for (Layer* layer : m_layer_stack)
			{
				layer->OnImGuiRenderer();
			}
			m_imgui_layer->End();
			m_window->Tick();
		}
	}

	void TomatoEngine::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(&TomatoEngine::OnWindowClose));
		Dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(&TomatoEngine::OnwindowResize));

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

	void TomatoEngine::PushOverLayer(Layer* overLay)
	{
		m_layer_stack.PushOverLayer(overLay);
	}

	void TomatoEngine::PopLayer(Layer* layer)
	{
		m_layer_stack.PopLayer(layer);
	}

	void TomatoEngine::PopOverLayer(Layer* overLay)
	{
		m_layer_stack.PopOverLayer(overLay);
	}

	bool TomatoEngine::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false;
	}

	bool TomatoEngine::OnwindowResize(WindowResizeEvent& e)
	{
		uint32_t width = e.GetWidth(), height = e.GetHeight();
		
		if (width == 0 || height == 0) {
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

			steady_clock::time_point tick_time_point = steady_clock::now();
			duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
			delta_time = time_span.count();

			m_last_tick_time_point = tick_time_point;
		}
		return delta_time;
	}


	void  TomatoEngine::CalculateFPS(float deltaTime)
	{
		m_average_duration = m_average_duration * (1 - k_fps_alpha) + deltaTime * k_fps_alpha;
		m_fps = static_cast<uint32_t> (1.0f / m_average_duration);
	}

}


