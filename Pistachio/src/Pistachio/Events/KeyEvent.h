#pragma once

#include "Event.h"

namespace Pistachio {

	class PISTACHIO_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode) 
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class PISTACHIO_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount) 
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepearCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent" << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
		/* macro equivalent to :
			static EventType GetStaticType() { return EventType::KeyPressed; }
			virtual EventType GetEventType() const override { return GetStaticType(); }
			virtual const char* GetName() const override { return "KeyPressed"; }
		*/

	private:
		int m_RepeatCount;
	};

	class PISTACHIO_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent" << m_KeyCode;
			return ss.str();
		};

		EVENT_CLASS_TYPE(KeyReleased)
	};

}