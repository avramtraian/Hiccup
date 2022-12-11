// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Window.h"

#include "KeyEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"

namespace HC
{

void Window::on_closed()
{
	m_is_pending_kill = true;

	WindowClosedEvent e;
	m_event_callback(e);
}

void Window::on_resized(uint32 width, uint32 height)
{
	m_width = width;
	m_height = height;

	m_dirty_width = m_width;
	m_dirty_height = m_height;

	WindowResizedEvent e = WindowResizedEvent(width, height);
	m_event_callback(e);
}

void Window::on_moved(int32 position_x, int32 position_y)
{
	m_position_x = position_x;
	m_position_y = position_y;

	m_dirty_position_x = m_position_x;
	m_dirty_position_y = m_position_y;

	WindowMovedEvent e = WindowMovedEvent(position_x, position_y);
	m_event_callback(e);
}

} // namespace HC