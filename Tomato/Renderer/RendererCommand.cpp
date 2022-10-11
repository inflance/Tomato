#include "RendererCommand.h"
#include "Tomato/Platform/OpenGL/OpenGLRendererAPI.h"


namespace Tomato {

	RendererAPI* RendererCommand::s_renderer_api = new OpenGLRendererAPI;
}

