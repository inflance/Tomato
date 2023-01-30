#include "windowsWindow.h"

#include <GLFW/glfw3.h>

#include "Tomato/Core/Core.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Events/KeyEvent.h"
#include "Tomato/Events/MouseEvent.h"
#include "Tomato/Renderer/RendererAPI.h"
#include "Tomato/Renderer/Vulkan/VulkanSwapChain.h"
#include "Tomato/Renderer/Vulkan/VulkanContext.h"
#include "Tomato/Renderer/Vulkan/VulkanDevice.h"
#include "Tomato/Renderer/Vulkan/VulkanContext.h"

namespace Tomato
{
	static unsigned char s_glfw_window_count = 0;

	static void glfwErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Tick()
	{
		glfwPollEvents();
		//m_context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (RendererAPI::GetCurrentAPI() == RendererAPI::API::OpenGL)
			glfwSwapInterval(enabled);

		m_window_data.IsVSync = enabled;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_window;
	}

	void WindowsWindow::SetWindowTitle(std::string_view title)
	{
		glfwSetWindowTitle(m_window, title.data());
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_window_data.Title = props.Title;
		m_window_data.Width = props.Width;
		m_window_data.Height = props.Height;
		m_window_data.IsVSync = props.IsVSync;
		m_window_data.IsFullScreen = props.IsFullScreen;


		if (s_glfw_window_count == 0)
		{
			const int success = glfwInit();
			LOG_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(glfwErrorCallback);

			++s_glfw_window_count;
		}
		if (RendererAPI::GetCurrentAPI() == RendererAPI::API::OpenGL)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		}
		else if (RendererAPI::GetCurrentAPI() == RendererAPI::API::Vulkan)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		if (m_window_data.IsFullScreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_window = glfwCreateWindow(mode->width, mode->height, m_window_data.Title.c_str(), primaryMonitor,
			                            nullptr);
		}
		else
		{
			m_window = glfwCreateWindow(m_window_data.Width, m_window_data.Height, m_window_data.Title.c_str(), nullptr,
			                            nullptr);
		}

		glfwSetWindowUserPointer(m_window, &m_window_data);
		SetVSync(m_window_data.IsVSync);

		//Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data.Width = width;
			data.Height = height;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;

			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
			case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
			case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
			case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((xOffset), (yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event((xPos), (yPos));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}
