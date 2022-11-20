-- Copyright (c) 2022-2022 Avram Traian. All rights reserved.

workspace "Hiccup"
	configurations
	{
		"Debug", "Release", "Shipping"
	}

	platforms
	{
		"Win64"
	}

	filter "platforms:Win64"
		system "Windows"
		architecture "x86_64"
	filter ""

	startproject "Hiccup-Editor"

	group "Core"
		include "Hiccup/Hiccup.lua"
	group "Tools"
		include "HiccupEd/HiccupEd.lua"
	group ""