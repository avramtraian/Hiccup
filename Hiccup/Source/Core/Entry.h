// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Application.h"

namespace HC
{

HC_API int32 GuardedMain(const ApplicationSpecification& appSpec, char** cmdArgs, uint32 cmdArgsCount);

bool CreateApplication(ApplicationSpecification* outAppSpec);

} // namespace HC

#if HC_CONFIGURATION_SHIPPING && HC_PLATFORM_WINDOWS
	#include <Windows.h>

	INT WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
	{
		HC::ApplicationSpecification appSpec = {};
		if (!HC::CreateApplication(&appSpec))
		{
			return -1;
		}

		return (INT)HC::GuardedMain(appSpec, __argv, (HC::uint32)(__argc));
	}
#else
	int main(int cmdArgsCount, char** cmdArgs)
	{
		HC::ApplicationSpecification appSpec = {};
		if (!HC::CreateApplication(&appSpec))
		{
			return -1;
		}

		return (int)HC::GuardedMain(appSpec, cmdArgs, (HC::uint32)cmdArgsCount);
	}
#endif // HC_CONFIGURATION_SHIPPING