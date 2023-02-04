-- Copyright (c) 2022-2023 Avram Traian. All rights reserved.

project "Hiccup-Core"
	kind "SharedLib"

	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	rtti "Off"
	exceptionhandling "Off"
	characterset "Unicode"

	targetname "Hiccup-Core"
	targetdir "%{wks.location}/Binaries/%{cfg.platform}-%{cfg.buildcfg}"
	objdir "%{wks.location}/Intermediate/Build/%{prj.name}/%{cfg.buildcfg}"

	pchheader "HiccupPCH.h"
	pchsource "Source/Core/HiccupPCH.cpp"

	files
	{
		"%{prj.location}/Source/**.h",
		"%{prj.location}/Source/**.cpp",

		"%{prj.location}/Hiccup.lua"
	}

	includedirs
	{
		"%{prj.location}/Source",

		(IncludeDirectories["VulkanSDK"])
	}

	forceincludes
	{
		"HiccupPCH.h"
	}

	defines
	{
		"HC_EXPORT_API"
	}

	links
	{
		(LibraryNames["VulkanSDK"])
	}

	libdirs
	{
		(LibraryPaths["VulkanSDK"])
	}

	filter "platforms:Win64"
		systemversion "latest"

		defines
		{
			"HC_PLATFORM_WIN64=1",
			"HC_PLATFORM_WINDOWS=1"
		}

	filter ""

	filter "configurations:Debug"
		optimize "Off"
		symbols "On"

		defines
		{
			"HC_CONFIGURATION_DEBUG=1"
		}

	filter "configurations:Release"
		optimize "On"
		symbols "On"

		defines
		{
			"HC_CONFIGURATION_RELEASE=1"
		}

	filter "configurations:Shipping"
		optimize "Speed"
		symbols "Off"

		defines
		{
			"HC_CONFIGURATION_SHIPPING=1"
		}

	filter ""