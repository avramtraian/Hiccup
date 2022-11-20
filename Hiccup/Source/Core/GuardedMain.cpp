// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Application.h"

namespace HC
{

HC_API int32 GuardedMain(const ApplicationSpecification& appSpec, char** cmdArgs, uint32 cmdArgsCount)
{
	Application* application = HcNew Application(appSpec);
	application->Run();
	HcDelete application;

	return 0;
}

} // namespace HC