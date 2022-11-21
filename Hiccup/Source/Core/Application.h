// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core.h"

namespace HC
{

HC_API bool ShouldRestartApplication();

struct ApplicationSpecification
{

};

class Application
{
public:
	HC_API static Application* Get() { return s_Instance; }

public:
	Application(const ApplicationSpecification& specification);
	~Application();

	void Run();
	void Close();

private:
	HC_API static Application* s_Instance;

private:
	ApplicationSpecification m_Specification;
	bool m_IsRunning = false;
};

} // namespace HC