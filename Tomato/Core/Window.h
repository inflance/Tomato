#pragma once

#include "Tomato/Events/Event.h"

namespace Tomato {

	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;

		WindowProps(const std::string& title = "window", int width = 1600, int height = 800)
			:Title(title), Width(width), Height(height){}
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
		[[nodiscard]] virtual bool IsVSync() const = 0;

		[[nodiscard]] virtual void* GetNativeWindow() const = 0;
		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}