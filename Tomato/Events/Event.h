#pragma once

#include <string>
#include <sstream>
#include <functional>
#include <iostream>

namespace Tomato{

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFoucus, WindowLostFoucus, WindowMoved,	//window event
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= 1 << 0,	//1		1
		EventCategoryInput			= 1 << 1,	//10	2
		EventCategoryKey			= 1 << 2,	//100	4
		EventCategoryMouse			= 1 << 3,	//1000	8
		EventCategoryMouseButton	= 1 << 4	//10000	16
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){ return EventType::type; }\
								virtual EventType GetEventType() const override{ return GetStaticType();}\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool handle = false;

		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const {
			return GetName();
		}

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category; //������ͬ������������1
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			:m_event(e) 
		{}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.handle |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;


	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}