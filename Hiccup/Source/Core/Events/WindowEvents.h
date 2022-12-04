// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Event.h"

namespace HC
{

class WindowClosedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::window_closed; }

public:
	WindowClosedEvent()
		: Event(get_static_type())
	{}
};

class WindowResizedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::window_resized; }

public:
	WindowResizedEvent(uint32 width, uint32 height)
		: Event(get_static_type())
		, m_width(width)
		, m_height(height)
	{}

public:
	ALWAYS_INLINE uint32 get_width() const { return m_width; }
	ALWAYS_INLINE uint32 get_height() const { return m_height; }

private:
	uint32 m_width;
	uint32 m_height;
};

class WindowMovedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::window_moved; }

public:
	WindowMovedEvent(int32 position_x, int32 position_y)
		: Event(get_static_type())
		, m_position_x(position_x)
		, m_position_y(position_y)
	{}

public:
	ALWAYS_INLINE int32 get_position_x() const { return m_position_x; }
	ALWAYS_INLINE int32 get_position_y() const { return m_position_y; }

private:
	int32 m_position_x;
	int32 m_position_y;
};

} // namespace HC