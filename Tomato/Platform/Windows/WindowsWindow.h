#pragma once

#include "Tomato/Core/Macro.h"
#include "Tomato/Core/Window.h"
#include "Tomato/Renderer/SwapChain.h"

struct GLFWwindow;

namespace Tomato
{
	
	class VulkanSwapChain;

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
		[[nodiscard]] inline bool IsVSync() const override { return m_window_data.IsVSync; };
		[[nodiscard]] inline bool IsFullScreen() const override { return m_window_data.IsFullScreen; }
		[[nodiscard]] inline void* GetNativeWindow() const override;
		void SetWindowTitle(std::string_view title) override;
		[[nodiscard]] inline const DeviceProps& GetDeviceProps() const override{ return m_device_props; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool IsVSync;
			bool IsFullScreen;
			EventCallbackFn EventCallback;
		};

		GLFWwindow* m_window;

		WindowData m_window_data;
		DeviceProps m_device_props;
	};
}
