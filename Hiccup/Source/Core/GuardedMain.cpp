// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Application.h"

#include "Core/Platform/Platform.h"
#include "Core/Memory/Memory.h"
#include "Core/Performance.h"
#include "Core/Logger.h"

namespace HC
{

HC_API int32 GuardedMain(const ApplicationSpecification& appSpec, char** cmdArgs, uint32 cmdArgsCount)
{
	// Shutdown graph.
	using PFN_Shutdown = void(*)(void);
	PFN_Shutdown systemShutdowns[4] = {};
	uint16 systemShudownsCount = 0;

	// Initializing the Platform system.
	PlatformSpecification platformSpec = {};
#if HC_CONFIGURATION_SHIPPING
	platformSpec.IsConsoleAttached = false;
#else
	platformSpec.IsConsoleAttached = true;
#endif
	if (!Platform::Initialize(platformSpec))
	{
		for (int32 i = systemShudownsCount - 1; i >= 0; --i)
		{
			systemShutdowns[i]();
		}
		return -2;
	}
	systemShutdowns[systemShudownsCount++] = Platform::Shutdown;

	// Initializing the Memory system.
	MemorySpecification memorySpec = {};
	memorySpec.should_initialize_tracker = true;
	if (!Memory::initialize(memorySpec))
	{
		for (int32 i = systemShudownsCount - 1; i >= 0; --i)
		{
			systemShutdowns[i]();
		}
		return -2;
	}
	systemShutdowns[systemShudownsCount++] = Memory::shutdown;

#if HC_ENABLE_PROFILING
	// Initializing the Performance Profiling Tool.
	ProfilerSpecification profilerSpec = {};
	if (!Profiler::Initialize(profilerSpec))
	{
		for (int32 i = systemShudownsCount - 1; i >= 0; --i)
		{
			systemShutdowns[i]();
		}
		return -2;
	}
	systemShutdowns[systemShudownsCount++] = Profiler::Shutdown;
#endif // HC_ENABLE_PROFILING

	// Initializing the Logging system.
	LoggerSpecification loggerSpec = {};
	if (!Logger::Initialize(loggerSpec))
	{
		for (int32 i = systemShudownsCount - 1; i >= 0; --i)
		{
			systemShutdowns[i]();
		}
		return -2;
	}
	systemShutdowns[systemShudownsCount++] = Logger::Shutdown;

	do
	{
		// Creating the application.
		Application* application = hc_new Application(appSpec);
		if (!application)
		{
			HC_LOG_FATAL("Failed to create the application instance! Aborting...");
			return -2;
		}

		// Running the application.
		application->Run();

		// Destroying the application.
		hc_delete application;
	}
	while (ShouldRestartApplication());

	// Shutting down the core systems.
	for (int32 i = systemShudownsCount - 1; i >= 0; --i)
	{
		systemShutdowns[i]();
	}

	return 0;
}

} // namespace HC