#include "tmtpch.h"

#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Tomato {

#ifdef TMT_PLATFORM_WINDOWS
	std::unique_ptr<Window> Window::Create(const WindowProps& props /*= WindowProps()*/)
	{
		return std::make_unique<WindowsWindow>(props);
	}
#endif

}
