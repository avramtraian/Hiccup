// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Event.h"

#include "Core/KeyCodes.h"

namespace HC
{

class KeyPressedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::key_pressed; }

public:
	KeyPressedEvent(KeyCode key)
		: Event(get_static_type())
		, m_key(key)
	{}

public:
	ALWAYS_INLINE KeyCode get_key() const { return m_key; }

private:
	KeyCode m_key;
};

class KeyReleasedEvent : public Event
{
public:
	ALWAYS_INLINE constexpr static EventType get_static_type() { return EventType::key_pressed; }

public:
	KeyReleasedEvent(KeyCode key)
		: Event(get_static_type())
		, m_key(key)
	{}

public:
	ALWAYS_INLINE KeyCode get_key() const { return m_key; }

private:
	KeyCode m_key;
};

} // namespace HC