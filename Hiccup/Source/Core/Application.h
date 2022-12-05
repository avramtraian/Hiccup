// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace HC
{

struct ApplicationDescription
{
	void (*on_event)(Event&);
};

class Application
{
public:
	HC_API static Application* get() { return s_instance; }

public:
	Application(const ApplicationDescription& description);
	~Application();

	void run();
	void close();

	HC_API void on_event(Event& e);

private:
	static bool on_key_pressed_event(const class KeyPressedEvent& e);
	static bool on_key_released_event(const class KeyReleasedEvent& e);

private:
	HC_API static Application* s_instance;

private:
	ApplicationDescription m_description;
	bool m_is_running = false;
};

} // namespace HC