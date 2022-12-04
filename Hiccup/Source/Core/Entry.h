// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Application.h"

namespace HC
{

HC_API int32 guarded_main(bool(*create_application_desc_callback)(ApplicationDescription*), char** cmd_args, uint32 cmd_args_count);

bool create_application_desc(ApplicationDescription* out_application_desc);

} // namespace HC

#if HC_CONFIGURATION_SHIPPING && HC_PLATFORM_WINDOWS
	#include <Windows.h>

	INT WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
	{
		return (INT)HC::guarded_main(HC::create_application_desc, __argv, (HC::uint32)(__argc));
	}
#else
	int main(int cmd_args_count, char** cmd_args)
	{
		return (int)HC::guarded_main(HC::create_application_desc, cmd_args, (HC::uint32)cmd_args_count);
	}
#endif // HC_CONFIGURATION_SHIPPING