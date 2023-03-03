#include "Window.h"

#include "Tomato/Platform/Windows/WindowsWindow.hpp"
#include "Tomato/Renderer/GraphicsContext.hpp"

namespace Tomato
{
#ifdef TMT_PLATFORM_WINDOWS
	std::shared_ptr<Window> Window::Create(const WindowProps& props /*= WindowProps()*/)
	{
		return std::make_shared<WindowsWindow>(props);
	}
#endif
}
