// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core.h"

namespace HC
{

HC_API bool should_restart_application();

struct ApplicationSpecification
{

};

class Application
{
public:
	HC_API static Application* get() { return s_instance; }

public:
	Application(const ApplicationSpecification& specification);
	~Application();

	void run();
	void close();

private:
	HC_API static Application* s_instance;

private:
	ApplicationSpecification m_specification;
	bool m_is_running = false;
};

} // namespace HC