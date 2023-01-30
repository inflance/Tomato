#pragma once

#include "Event.h"

namespace Tomato
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_mouse_x(x), m_mouse_y(y)
		{
		}

		[[nodiscard]] float GetX() const { return m_mouse_x; }
		[[nodiscard]] float GetY() const { return m_mouse_y; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float m_mouse_x, m_mouse_y;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float x_offset, const float y_offset)
			: m_x_offset(x_offset), m_y_offset(y_offset)
		{
		}

		[[nodiscard]] float GetXOffset() const { return m_x_offset; }
		[[nodiscard]] float GetYOffset() const { return m_y_offset; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << std::string("MouseScrolledEvent: ") << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_x_offset, m_y_offset;
	};

	class MouseButtOnEvent : public Event
	{
	public:
		[[nodiscard]] int GetMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton);

	protected:
		MouseButtOnEvent(const int button)
			: m_button(button)
		{
		}

		int m_button;
	};

	class MouseButtonPressedEvent : public MouseButtOnEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtOnEvent(button)
		{
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class MouseButtonReleasedEvent : public MouseButtOnEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtOnEvent(button)
		{
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}
