#pragma once

#include "Macro.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Renderer/SwapChain.h"

namespace Tomato
{
	class GraphicsContext;

	struct DeviceProps
	{
		uint32_t api_version;
		std::string vendor_name;
		std::string device_name;
	};

	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;
		bool IsVSync;
		bool IsFullScreen;

		WindowProps(std::string title = "Window", int width = 1600, int height = 800)
			: Title(std::move(title)), Width(width), Height(height), IsVSync(false), IsFullScreen(false)
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
