#pragma once

#include "Tomato/Core/Core.h"
#include "Tomato/Events/ApplicationEvent.h"
#include "Tomato/Events/Event.h"
#include "Tomato/Events/KeyEvent.h"
#include "Tomato/Events/MouseEvent.h"

namespace Tomato
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnCreate()
		{
		}

		virtual void OnDestroy()
		{
		}

		virtual void Tick(float delta_time)
		{
		}

		virtual void OnImGuiRenderer()
		{
		}

		virtual void OnEvent(Event& event)
		{
		}

		const std::string& GetName() const { return m_debug_name; }

	protected:
		std::string m_debug_name;
	};
}
