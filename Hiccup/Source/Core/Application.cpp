// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Application.h"

#include "Engine/KeyEvents.h"
#include "Engine/MouseEvents.h"
#include "Engine/WindowEvents.h"

#include "Renderer/Renderer.h"

namespace HC
{

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationDescription& description)
	: m_description(description)
{
	s_instance = this;

	if (m_description.window_description.event_callback == nullptr)
	{
		m_description.window_description.event_callback = [](Event& e) { Application::get()->on_event(e); };
	}
	m_primary_window = Window::create(m_description.window_description);

	RendererDescription renderer_desc = {};
	renderer_desc.rhi = RHI::vulkan;
	renderer_desc.enable_validation = true;
	HC_VERIFYF(Renderer::initialize(renderer_desc), "Failed to initialize the renderer!");
}

Application::~Application()
{
	Renderer::shutdown();

	m_primary_window.release();
	s_instance = nullptr;
}

void Application::run()
{
	m_is_running = true;

	while (m_is_running)
	{
		HC_PROFILE_BEGIN_FRAME;

		m_primary_window->update_window();

		if (m_primary_window->is_pending_kill())
		{
			close();
		}

		HC_PROFILE_END_FRAME;
	}

	m_is_running = false;
}

void Application::close()
{
	m_is_running = false;
}

void Application::on_event(Event& e)
{
	EventDispatcher dispatcher = EventDispatcher(e);

	dispatcher.Dispatch<KeyPressedEvent>(Application::on_key_pressed_event);
	dispatcher.Dispatch<KeyReleasedEvent>(Application::on_key_released_event);

	if (m_description.on_event)
	{
		m_description.on_event(e);
	}
}

bool Application::on_key_pressed_event(const KeyPressedEvent& e)
{
	return false;
}

bool Application::on_key_released_event(const KeyReleasedEvent& e)
{
	return false;
}

} // namespace HC