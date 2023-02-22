#include "Engine.h"

#include <glm/glm.hpp>

#include "Timer.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Platform/Windows/WindowsWindow.h"
#include "Tomato/Renderer/Renderer.h"
#include "Tomato/World/World.h"

namespace Tomato
{
	void Engine::StartUp()
	{
		try
		{
			LOG_TRACE("Engine StartUp");

			//Window Init
			m_window = Window::Create();
			m_window->SetVSync(true);
			m_window->SetEventCallback(BIND_EVENT_FUNC(Engine::OnEvent));
			m_context = GraphicsContext::Create(m_window.get());
			m_context->Init();
			const auto props = m_window->GetDeviceProps();
			m_device_props = props;
			m_device_str = std::format("Tomato Engine - {} - ", m_device_props.device_name_);
			//Renderer Init
			Renderer::Init(m_context);
			Renderer::OnWindowResize(0, 0, m_window->GetWidth(), m_window->GetHeight());
		}
		catch (vk::SystemError& err)
		{
			std::cout << "vk::SystemError: " << err.what() << std::endl;
			exit(-1);
		}
		catch (std::exception& err)
		{
			std::cout << "std::exception: " << err.what() << std::endl;
			exit(-1);
		}
		catch (...)
		{
			std::cout << "unknown error\n";
			exit(-1);
		}

		//ImGuiLayer Init
		m_imgui_layer = ImGuiLayer::Create();
		PushOverLayer(m_imgui_layer);
	}

	void Engine::ShutDown()
	{
		LOG_INFO("Engine ShutDown");
		Renderer::WaitAndRender();
		Renderer::Destroy();
	}

	void Engine::Run()
	{
		LOG_TRACE("Engine Run");
		static Timer timer;

		while (m_running)
		{
			m_window->Tick();
			const float delta_time = CalculateDeltaTime();
			CalculateFPS(delta_time);

			if (!m_minimized)
			{
				m_context->Begin();

				auto app = this;

				m_world.LogicTick(delta_time);

				for (Layer* layer : m_layer_stack)
				{
					layer->Tick(delta_time);
				}
				Renderer::Submit([app]
				{
					app->m_imgui_layer->Begin();
					for (Layer* layer : app->m_layer_stack)
					{
						layer->OnImGuiRenderer();
					}
					app->m_imgui_layer->End();
				});

				Renderer::WaitAndRender();
				m_context->Present();
			}

			if (timer.CountTime() > 1000.0f)
			{
				m_window->SetWindowTitle(std::format("{1}{0} fps", m_fps, m_device_str));
				timer.Reset();
			}
		}
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Engine::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(Engine::OnWindowResize));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.handle)
				break;
		}
	}

	void Engine::Close()
	{
		m_running = false;
	}

	void Engine::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
	}

	void Engine::PushOverLayer(Layer* over_layer)
	{
		m_layer_stack.PushOverLayer(over_layer);
	}

	void Engine::PopLayer(Layer* layer)
	{
		m_layer_stack.PopLayer(layer);
	}

	void Engine::PopOverLayer(Layer* over_layer)
	{
		m_layer_stack.PopOverLayer(over_layer);
	}

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false;
	}

	bool Engine::OnWindowResize(WindowResizeEvent& e)
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

	float Engine::CalculateDeltaTime()
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

	void Engine::CalculateFPS(float delta_time)
	{
		m_average_duration = m_average_duration * (1 - k_fps_alpha) + delta_time * k_fps_alpha;
		m_fps = static_cast<uint32_t>(1.0f / m_average_duration);
	}
}
