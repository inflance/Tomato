#pragma once

#include <string>
#include <sstream>

namespace Tomato
{
	enum class EventType
	{
		None = 0,
		//window event
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		//app tick
		AppTick,
		AppUpdate,
		AppRender,
		//key event
		KeyPressed,
		KeyReleased,
		KeyTyped,
		//mouse event
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None						= 0,
		EventCategoryApplication	= 1 << 0,
		EventCategoryInput			= 1 << 1,
		EventCategoryKey			= 1 << 2,
		EventCategoryMouse			= 1 << 3,
		EventCategoryMouseButton	= 1 << 4 //10000	16
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType(){ return EventType::type; }\
								[[nodiscard]] virtual EventType GetEventType() const override{ return GetStaticType();}\
								[[nodiscard]] virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) [[nodiscard]] virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;
		bool handle = false;

		[[nodiscard]] virtual EventType GetEventType() const = 0;
		[[nodiscard]] virtual const char* GetName() const = 0;
		[[nodiscard]] virtual int GetCategoryFlags() const = 0;
		[[nodiscard]] virtual std::string ToString() const { return GetName(); }

		[[nodiscard]] bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			: m_event(e)
		{
		}

		template <typename T, typename F>
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
