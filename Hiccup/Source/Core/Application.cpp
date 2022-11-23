// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Application.h"

namespace HC
{

static bool s_RestartApplication = false;

bool ShouldRestartApplication()
{
	return s_RestartApplication;
}

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification)
{
	s_Instance = this;
}

Application::~Application()
{
	s_Instance = nullptr;
}

void Application::Run()
{
	m_IsRunning = true;

	while (m_IsRunning)
	{
		HC_PROFILE_BEGIN_FRAME;

		

		HC_PROFILE_END_FRAME;
	}

	m_IsRunning = false;
}

void Application::Close()
{
	m_IsRunning = false;
}

} // namespace HC