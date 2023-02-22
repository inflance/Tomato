#pragma once

#include "Macro.h"
#include "Tomato/Events/Event.h"

namespace Tomato
{
	class GraphicsContext;

	struct DeviceProps
	{
		uint32_t api_version_;
		std::string vendor_name_;
		std::string device_name_;
	};

	struct WindowProps
	{
		std::string title_;
		int width_;
		int height_;
		bool is_v_sync_;
		bool is_full_screen_;

		WindowProps(std::string title = "Window", int width = 1600, int height = 800)
			: title_(std::move(title)), width_(width), height_(height), is_v_sync_(false), is_full_screen_(false)
		{
		}
	};

	using EventCallbackFn = std::function<void(Event&)>;

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Tick() = 0;
		[[nodiscard]] virtual unsigned int GetWidth() const = 0;
		[[nodiscard]] virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetWindowTitle(std::string_view title) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;

		[[nodiscard]] virtual void* GetNativeWindow() const = 0;
		[[nodiscard]] virtual bool IsFullScreen() const = 0;
		[[nodiscard]] virtual const inline DeviceProps& GetDeviceProps() const = 0;
		static std::shared_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}
