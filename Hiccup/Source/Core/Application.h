// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core.h"
#include "Engine/Event.h"
#include "Engine/Window.h"

namespace HC
{

struct ApplicationDescription
{
    void (*on_event)(Event&);

    WindowDescription window_description;
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

public:
    HC_API Window* get_primary_window() { return m_primary_window.get(); }

private:
    static bool on_key_pressed_event(const class KeyPressedEvent& e);
    static bool on_key_released_event(const class KeyReleasedEvent& e);

private:
    HC_API static Application* s_instance;

private:
    ApplicationDescription m_description;
    bool m_is_running = false;
    UniquePtr<Window> m_primary_window;
};

} // namespace HC