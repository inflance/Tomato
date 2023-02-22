#include "RendererCommand.h"

#include "Tomato/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Tomato {

	std::shared_ptr<RendererAPI> RendererCommand::s_renderer_api = std::make_shared<OpenGLRendererAPI>();
}

