#pragma once

#include "Event.h"

namespace Tomato {

	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const{
			return m_key_code;
		}

		EVENT_CLASS_CATEGORY(EventCategoryKey | EventCategoryInput)
	protected:
		int m_key_code;

		KeyEvent(int keycode)
			:m_key_code(keycode)
		{}
	
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			:KeyEvent(keycode), m_repeatCount(repeatCount)
		{}

		inline int getRepeatCount() const {
			return m_repeatCount;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressed:" << m_key_code << "{" << m_repeatCount << "repesats}";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent 
	{
	public:
		KeyReleasedEvent(const int& keycode)
			:KeyEvent(keycode)
		{}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleased:" << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
	
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			:KeyEvent(keycode) {}


		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}