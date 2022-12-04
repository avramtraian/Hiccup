// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Application.h"

#include "Core/Platform/Platform.h"
#include "Core/Memory/Memory.h"
#include "Core/Performance.h"
#include "Core/Logger.h"

namespace HC
{

HC_API int32 guarded_main(const ApplicationDescription& application_desc, char** cmd_args, uint32 cmd_args_count)
{
	// Shutdown graph.
	using PFN_Shutdown = void(*)(void);
	PFN_Shutdown system_shutdowns[4] = {};
	uint16 system_shutdowns_count = 0;

	// Initializing the Platform system.
	PlatformSpecification platform_spec = {};
#if HC_CONFIGURATION_SHIPPING
	platform_spec.is_console_attached = false;
#else
	platform_spec.is_console_attached = true;
#endif
	if (!Platform::initialize(platform_spec))
	{
		for (int32 i = system_shutdowns_count - 1; i >= 0; --i)
		{
			system_shutdowns[i]();
		}
		return -2;
	}
	system_shutdowns[system_shutdowns_count++] = Platform::shutdown;

	// Initializing the Memory system.
	MemorySpecification memory_spec = {};
	memory_spec.should_initialize_tracker = true;
	if (!Memory::initialize(memory_spec))
	{
		for (int32 i = system_shutdowns_count - 1; i >= 0; --i)
		{
			system_shutdowns[i]();
		}
		return -2;
	}
	system_shutdowns[system_shutdowns_count++] = Memory::shutdown;

#if HC_ENABLE_PROFILING
	// Initializing the Performance Profiling Tool.
	ProfilerSpecification profiler_spec = {};
	if (!Profiler::initializer(profiler_spec))
	{
		for (int32 i = system_shutdowns_count - 1; i >= 0; --i)
		{
			system_shutdowns[i]();
		}
		return -2;
	}
	system_shutdowns[system_shutdowns_count++] = Profiler::shutdown;
#endif // HC_ENABLE_PROFILING

	// Initializing the Logging system.
	LoggerSpecification logger_spec = {};
	if (!Logger::initialize(logger_spec))
	{
		for (int32 i = system_shutdowns_count - 1; i >= 0; --i)
		{
			system_shutdowns[i]();
		}
		return -2;
	}
	system_shutdowns[system_shutdowns_count++] = Logger::shutdown;

	do
	{
		// Creating the application.
		Application* application = hc_new Application(application_desc);
		if (!application)
		{
			HC_LOG_FATAL("Failed to create the application instance! Aborting...");
			return -2;
		}

		// Running the application.
		application->run();

		// Destroying the application.
		hc_delete application;
	}
	while (should_restart_application());

	// Shutting down the core systems.
	for (int32 i = system_shutdowns_count - 1; i >= 0; --i)
	{
		system_shutdowns[i]();
	}

	return 0;
}

} // namespace HC