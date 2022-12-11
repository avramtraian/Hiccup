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
	out_application_desc->window_description.view_mode = WindowViewMode::windowed;
	out_application_desc->window_description.start_mode = WindowStartMode::maximized;
	return true;
}

} // namespace HC