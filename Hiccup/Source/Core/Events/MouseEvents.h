// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Event.h"

#include "Core/KeyCodes.h"

namespace HC
{

class MouseMovedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::mouse_moved; }

public:
	MouseMovedEvent(int32 position_x, int32 position_y)
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

class MouseButtonPressed : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::mouse_button_pressed; }

public:
	MouseButtonPressed(MouseButton button)
		: Event(get_static_type())
		, m_button(button)
	{}

public:
	ALWAYS_INLINE MouseButton get_button() const { return m_button; }

private:
	MouseButton m_button;
};

class MouseButtonReleased : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::mouse_button_released; }

public:
	MouseButtonReleased(MouseButton button)
		: Event(get_static_type())
		, m_button(button)
	{}

public:
	ALWAYS_INLINE MouseButton get_button() const { return m_button; }

private:
	MouseButton m_button;
};

class MouseWheelScrolledEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::mouse_wheel_scrolled; }

public:
	MouseWheelScrolledEvent(int32 delta)
		: Event(get_static_type())
		, m_delta(delta)
	{}

public:
	ALWAYS_INLINE int32 get_delta() const { return m_delta; }

private:
	int32 m_delta;
};

} // namespace HC