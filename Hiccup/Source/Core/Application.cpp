// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Application.h"

namespace HC
{

static bool s_RestartApplication = false;

bool should_restart_application()
{
	return s_RestartApplication;
}

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
	: m_specification(specification)
{
	s_instance = this;
}

Application::~Application()
{
	s_instance = nullptr;
}

void Application::run()
{
	m_is_running = true;

	while (m_is_running)
	{
		HC_PROFILE_BEGIN_FRAME;



		HC_PROFILE_END_FRAME;
	}

	m_is_running = false;
}

void Application::close()
{
	m_is_running = false;
}

} // namespace HC