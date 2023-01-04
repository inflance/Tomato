#pragma once

#include "Tomato/Core/Window.h"
#include "Tomato/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Tomato{

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;
		EventCallbackFn EventCallback;
	};

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void Tick() override;
		unsigned int GetWidth() const override { return m_window_data.Width; }
		unsigned int GetHeight() const override { return m_window_data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_window_data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void* GetNativeWindow() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_window;
		std::shared_ptr<GraphicsContext> m_context;

		WindowData m_window_data;
	};
}