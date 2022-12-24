// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Application.h"

#include "Core/Platform/Platform.h"
#include "Core/Memory/Memory.h"
#include "Core/Performance.h"
#include "Core/Logger.h"

namespace HC
{

#define HC_INITIALIZE(SYSTEM_NAME, SYSTEM_DESCRIPTION)          \
	if (!SYSTEM_NAME::initialize(SYSTEM_DESCRIPTION))           \
	{	                                                        \
		for (int32_t i = system_shutdowns_count - 1; i >= 0; --i) \
		{                                                       \
			system_shutdowns[i]();                              \
		}                                                       \
		return EXIT_FAILURE;                                    \
	}                                                           \
	system_shutdowns[system_shutdowns_count++] = SYSTEM_NAME::shutdown;

HC_API int32_t guarded_main(bool(*create_application_desc_callback)(ApplicationDescription*), char** cmd_args, uint32_t cmd_args_count)
{
	// Shutdown graph.
	using PFN_Shutdown = void(*)(void);
	PFN_Shutdown system_shutdowns[4] = {};
	uint16_t system_shutdowns_count = 0;

	//---------------- Initializing the Platform system ----------------
	PlatformDescription platform_desc = {};
#if HC_CONFIGURATION_SHIPPING
	platform_desc.is_console_attached = false;
#else
	platform_desc.is_console_attached = true;
#endif
	HC_INITIALIZE(Platform, platform_desc);
	//------------------------------------------------------------------


	//---------------- Initializing the Memory system ----------------
	MemoryDescription memory_desc = {};
	memory_desc.should_initialize_tracker = true;
	HC_INITIALIZE(Memory, memory_desc);
	//----------------------------------------------------------------


	//---------------- Initializing the Performance Profiling Tool ----------------
#if HC_ENABLE_PROFILING
	ProfilerDescription profiler_desc = {};
	HC_INITIALIZE(Profiler, profiler_desc);
#endif // HC_ENABLE_PROFILING
	//-----------------------------------------------------------------------------


	//---------------- Initializing the Logging system ----------------
	LoggerDescription logger_desc = {};
	HC_INITIALIZE(Logger, logger_desc);
	//-----------------------------------------------------------------

	// Creating the application description.
	ApplicationDescription application_desc = {};
	if (!create_application_desc_callback || !create_application_desc_callback(&application_desc))
	{
		HC_LOG_FATAL("Failed to create the application description! Aborting...");
		return EXIT_FAILURE;
	}

	// Creating the application instance.
	Application* application = hc_new Application(application_desc);
	if (!application)
	{
		HC_LOG_FATAL("Failed to create the application instance! Aborting...");
		return EXIT_FAILURE;
	}

	// Running the application.
	application->run();

	// Destroying the application.
	hc_delete application;

	// Shutting down the core systems.
	for (int32_t i = system_shutdowns_count - 1; i >= 0; --i)
	{
		system_shutdowns[i]();
	}

	return EXIT_SUCCESS;
}

} // namespace HC