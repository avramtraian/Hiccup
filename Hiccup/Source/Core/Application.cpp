// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Application.h"

namespace HC
{

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& appSpec)
	: m_Specification(appSpec)
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

	}

	m_IsRunning = false;
}

void Application::Close()
{
	m_IsRunning = false;
}

} // namespace HC