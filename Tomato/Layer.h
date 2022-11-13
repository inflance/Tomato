#pragma once

#include "Core.h"

//event
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
//event
#include "TimeSpan.h"


#include <string>

namespace Tomato {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach(){}//∏Ωº”
		virtual void OnDetach(){}//∑÷¿Î
		virtual void Tick(TimeSpan ts){}
		virtual void OnImGuiRender(){}
		virtual void OnEvent(Event& event){}

		inline const std::string& GetName() const {
			return m_debug_name;
		}
	protected:
		std::string m_debug_name;
	};
}