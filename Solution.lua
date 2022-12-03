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

	LibraryNames = {}
	LibraryPaths = {}
	IncludeDirectories = {}
	
	VulkanPath = os.getenv("VULKAN_SDK");
	LibraryNames["VulkanSDK"] = "vulkan-1.lib";
	LibraryPaths["VulkanSDK"] = (VulkanPath.."/Lib")
	IncludeDirectories["VulkanSDK"] = (VulkanPath.."/Include")

	group "Core"
		include "Hiccup/Hiccup.lua"
	group "Tools"
		include "HiccupEd/HiccupEd.lua"
	group ""