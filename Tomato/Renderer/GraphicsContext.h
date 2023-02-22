#pragma once

namespace Tomato
{
	class Window;

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void Destroy() = 0;
		virtual void Present() = 0;
		virtual void OnResize(uint32_t width, uint32_t height) = 0;

		static std::shared_ptr<GraphicsContext> Create(Window* window);

	protected:
	};
}
