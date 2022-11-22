// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Application.h"

#include "Core/Platform/Platform.h"
#include "Core/Memory.h"
#include "Core/Logger.h"

namespace HC
{

HC_API int32 GuardedMain(const ApplicationSpecification& appSpec, char** cmdArgs, uint32 cmdArgsCount)
{
	// Initializing the Platform system.
	PlatformSpecification platformSpec = {};
#if HC_CONFIGURATION_SHIPPING
	platformSpec.IsConsoleAttached = false;
#else
	platformSpec.IsConsoleAttached = true;
#endif
	if (!Platform::Initialize(platformSpec))
	{
		return -2;
	}

	// Initializing the Memory system.
	MemorySpecification memorySpec = {};
	memorySpec.ShouldInitializeTracker = true;
	if (!Memory::Initialize(memorySpec))
	{
		Platform::Shutdown();
		return -2;
	}

	// Initializing the Logging system.
	LoggerSpecification loggerSpec = {};
	if (!Logger::Initialize(loggerSpec))
	{
		Platform::Shutdown();
		Memory::Shutdown();
		return -2;
	}

	do
	{
		// Creating the application.
		Application* application = HcNew Application(appSpec);
		if (!application)
		{
			HC_LOG_FATAL("Failed to create the application instance! Aborting...");
			return -2;
		}

		// Running the application.
		application->Run();

		// Destroying the application.
		HcDelete application;
	}
	while (ShouldRestartApplication());

	// Shutting down the core systems.
	Logger::Shutdown();
	Memory::Shutdown();
	Platform::Shutdown();

	return 0;
}

} // namespace HC