-- Copyright (c) 2022-2023 Avram Traian. All rights reserved.

project "Hiccup-Editor"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    rtti "Off"
    exceptionhandling "Off"
    characterset "Unicode"

    targetname "Hiccup-Editor"
    targetdir "%{wks.location}/Binaries/%{cfg.platform}-%{cfg.buildcfg}"
    objdir "%{wks.location}/Intermediate/Build/%{prj.name}/%{cfg.buildcfg}"

    files
    {
        "%{prj.location}/Source/**.h",
        "%{prj.location}/Source/**.cpp",

        "%{prj.location}/HiccupEd.lua"
    }

    includedirs
    {
        "%{prj.location}/Source",

        "%{wks.location}/Hiccup/Source"
    }

    links
    {
        "Hiccup-Core"
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

        kind "ConsoleApp"

        defines
        {
            "HC_CONFIGURATION_DEBUG=1"
        }

    filter "configurations:Release"
        optimize "On"
        symbols "On"

        kind "ConsoleApp"

        defines
        {
            "HC_CONFIGURATION_RELEASE=1"
        }

    filter "configurations:Shipping"
        optimize "Speed"
        symbols "Off"

        kind "WindowedApp"

        defines
        {
            "HC_CONFIGURATION_SHIPPING=1"
        }

    filter ""