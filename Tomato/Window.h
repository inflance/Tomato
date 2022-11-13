#pragma once

#include "Events/Event.h"

namespace Tomato {

	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;

		WindowProps(const std::string& title = "Tomato Engine",
			int width = 1600,
			int height = 800)
			:Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Tick() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}