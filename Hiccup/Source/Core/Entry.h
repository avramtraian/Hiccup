// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Application.h"

namespace HC
{

HC_API int32 guarded_main(const ApplicationDescription& application_desc, char** cmd_args, uint32 cmd_args_count);

bool create_application(ApplicationDescription* out_application_desc);

} // namespace HC

#if HC_CONFIGURATION_SHIPPING && HC_PLATFORM_WINDOWS
	#include <Windows.h>

	INT WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
	{
		HC::ApplicationDescription application_desc = {};
		if (!HC::create_application(&application_desc))
		{
			return -1;
		}

		return (INT)HC::guarded_main(application_desc, __argv, (HC::uint32)(__argc));
	}
#else
	int main(int cmd_args_count, char** cmd_args)
	{
		HC::ApplicationDescription application_desc = {};
		if (!HC::create_application(&application_desc))
		{
			return -1;
		}

		return (int)HC::guarded_main(application_desc, cmd_args, (HC::uint32)cmd_args_count);
	}
#endif // HC_CONFIGURATION_SHIPPING