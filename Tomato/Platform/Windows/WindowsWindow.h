#pragma once

#include "Tomato/Core/Macro.h"
#include "Tomato/Core/Window.h"

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
		unsigned int GetWidth() const override { return m_window_data.width_; }
		unsigned int GetHeight() const override { return m_window_data.height_; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_window_data.event_callback_ = callback; }

		void SetVSync(bool enabled) override;
		[[nodiscard]] bool IsVSync() const override { return m_window_data.is_v_sync_; };
		[[nodiscard]] bool IsFullScreen() const override { return m_window_data.is_full_screen_; }
		[[nodiscard]] inline void* GetNativeWindow() const override;
		void SetWindowTitle(std::string_view title) override;
		[[nodiscard]] const DeviceProps& GetDeviceProps() const override { return m_device_props; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		struct WindowData
		{
			std::string title_{};
			uint32_t width_{}, height_{};
			bool is_v_sync_{};
			bool is_full_screen_{};
			EventCallbackFn event_callback_;
		};

		GLFWwindow* m_window;

		WindowData m_window_data;
		DeviceProps m_device_props;
		Ref<GraphicsContext> m_context;
	};
}
