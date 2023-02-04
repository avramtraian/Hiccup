// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

enum class EventType : uint16_t
{
	WindowClosed,
	WindowResized,
	WindowMoved,

	MouseMoved,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseWheelScrolled,

	KeyPressed,
	KeyReleased
};

class Event
{
protected:
	Event(EventType event_type)
		: m_event_type(event_type)
		, m_is_handled(false)
	{}

public:
	ALWAYS_INLINE EventType get_type() const { return m_event_type; }

private:
	EventType m_event_type;
	bool m_is_handled;

private:
	friend class EventDispatcher;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& e)
		: m_event(e)
	{}

public:
	template<typename EventType>
	ALWAYS_INLINE bool Dispatch(bool(*callback)(const EventType&))
	{
		if ((m_event.get_type() == EventType::get_static_type()) && (!m_event.m_is_handled))
		{
			m_event.m_is_handled = callback((const EventType&)m_event);
		}

		return m_event.m_is_handled;
	}

private:
	Event& m_event;
};

} // namespace HC