#pragma once

#include "Event.h"
#include <sstream>

namespace Tomato {

	class KeyEvent : public Event
	{
	public:
		[[nodiscard]] int GetKeyCode() const{return m_key_code;}

		EVENT_CLASS_CATEGORY(EventCategoryKey | EventCategoryInput)
	protected:
		int m_key_code;

		KeyEvent(const int key_code)
			:m_key_code(key_code){}
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int key_code, const int repeat_count)
			:KeyEvent(key_code), m_repeat_count(repeat_count){}

		[[nodiscard]] int GetRepeatCount() const {return m_repeat_count;}

		[[nodiscard]] std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressed:" << m_key_code << "{" << m_repeat_count << "repeats}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_repeat_count;
	};

	class KeyReleasedEvent : public KeyEvent 
	{
	public:
		KeyReleasedEvent(const int key_code)
			:KeyEvent(key_code){}

		[[nodiscard]] std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleased:" << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
	
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int key_code)
			:KeyEvent(key_code) {}

		[[nodiscard]] std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}