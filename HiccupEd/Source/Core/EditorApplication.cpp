// Copyright(c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Core.h"
#include "Core/Entry.h"

namespace HC
{

bool create_application_desc(ApplicationDescription* out_application_desc)
{
	out_application_desc->window_description.width = 1280;
	out_application_desc->window_description.height = 720;
	out_application_desc->window_description.position_x = 300;
	out_application_desc->window_description.position_y = 200;
	out_application_desc->window_description.view_mode = WindowViewMode::Windowed;
	out_application_desc->window_description.start_mode = WindowStartMode::Maximized;

#if HC_CONFIGURATION_DEBUG
	out_application_desc->window_description.title = "Hiccup Editor --- Platform: Win64, Configuration: Debug --- Untitled*"sv;
#elif HC_CONFIGURATION_RELEASE
	out_application_desc->window_description.title = "Hiccup Editor --- Platform: Win64, Configuration: Release --- Untitled*"sv;
#elif HC_CONFIGURATION_SHIPPING
	out_application_desc->window_description.title = "Hiccup Editor --- Platform: Win64, Configuration: Shipping --- Untitled*"sv;
#endif

	return true;
}

} // namespace HC