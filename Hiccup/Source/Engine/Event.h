// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

enum class EventType : uint16
{
	window_closed,
	window_resized,
	window_moved,

	mouse_moved,
	mouse_button_pressed,
	mouse_button_released,
	mouse_wheel_scrolled,

	key_pressed,
	key_released
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