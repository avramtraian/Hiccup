// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Platform/Platform.h"

#include "Core/Memory/Memory.h"

#include <Windows.h>
#include <cstdlib>

namespace HC
{

struct WindowsPlatformData
{
	PlatformSpecification specification;

	uint64 performance_tick_frequency;
	uint64 initialization_nanoseconds;

	HANDLE console_handle;
	Platform::ConsoleColor console_foreground;
	Platform::ConsoleColor console_background;
};
static_internal WindowsPlatformData* s_platform_data = nullptr;

bool Platform::initialize(const PlatformSpecification& specification)
{
	s_platform_data = (WindowsPlatformData*)std::malloc(sizeof(WindowsPlatformData));
	if (!s_platform_data)
	{
		return false;
	}

	new (s_platform_data) WindowsPlatformData();

	s_platform_data->specification = specification;

	LARGE_INTEGER performance_tick_frequency;
	if (!QueryPerformanceFrequency(&performance_tick_frequency))
	{
		return false;
	}
	s_platform_data->performance_tick_frequency = performance_tick_frequency.QuadPart;

	s_platform_data->initialization_nanoseconds = get_nanoseconds();

	if (s_platform_data->specification.is_console_attached)
	{
		s_platform_data->console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		s_platform_data->console_foreground = ConsoleColor::max_enum_value;
		s_platform_data->console_background = ConsoleColor::max_enum_value;
		set_console_color(ConsoleColor::light_gray, ConsoleColor::black);
	}

	return true;
}

void Platform::shutdown()
{
	if (s_platform_data->specification.is_console_attached)
	{
		set_console_color(ConsoleColor::light_gray, ConsoleColor::black);
	}

	s_platform_data->~WindowsPlatformData();
	std::free(s_platform_data);
	s_platform_data = nullptr;
}

void* Platform::allocate_memory(usize bytes_count)
{
	return std::malloc(bytes_count);
}

void Platform::free_memory(void* memory_block)
{
	std::free(memory_block);
}

uint64 Platform::get_performance_tick_count()
{
	LARGE_INTEGER performance_counter;
	QueryPerformanceCounter(&performance_counter);
	return performance_counter.QuadPart;
}

uint64 Platform::get_performance_tick_frequency()
{
	return s_platform_data->performance_tick_frequency;
}

uint64 Platform::get_nanoseconds()
{
	const uint64 ticks = get_performance_tick_count();
	const uint64 ticks_freq = s_platform_data->performance_tick_frequency;

	const float64 nanoseconds = ((float64)ticks * 1e9) / (float64)ticks_freq;
	return (uint64)nanoseconds;
}

uint64 Platform::get_nanoseconds_since_initialization()
{
	return get_nanoseconds() - s_platform_data->initialization_nanoseconds;
}

void Platform::set_console_color(ConsoleColor foreground, ConsoleColor background)
{
	if (!s_platform_data->specification.is_console_attached)
	{
		return;
	}
	 
	if (s_platform_data->console_foreground == foreground && s_platform_data->console_background == background)
	{
		return;
	}

	s_platform_data->console_foreground = foreground;
	s_platform_data->console_background = background;

	const WORD console_color = (WORD)s_platform_data->console_foreground | ((WORD)s_platform_data->console_background << 4);
	SetConsoleTextAttribute(s_platform_data->console_handle, console_color);
}

void Platform::write_to_console(const char* message, usize message_length)
{
	if (!s_platform_data->specification.is_console_attached)
	{
		return;
	}

	WriteConsoleA(s_platform_data->console_handle, message, (DWORD)message_length, NULL, NULL);
}

void Platform::get_local_system_time(SystemTime* out_system_time)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	out_system_time->year = systemTime.wYear;
	out_system_time->month = systemTime.wMonth;
	out_system_time->day = systemTime.wDay;
	out_system_time->hour = systemTime.wHour;
	out_system_time->second = systemTime.wSecond;
	out_system_time->minute = systemTime.wMinute;
	out_system_time->millisecond = systemTime.wMilliseconds;
}

void Platform::get_global_system_time(SystemTime* out_system_time)
{
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	out_system_time->year = systemTime.wYear;
	out_system_time->month = systemTime.wMonth;
	out_system_time->day = systemTime.wDay;
	out_system_time->hour = systemTime.wHour;
	out_system_time->second = systemTime.wSecond;
	out_system_time->minute = systemTime.wMinute;
	out_system_time->millisecond = systemTime.wMilliseconds;
}

uint32 Platform::open_popup(const char* title, const char* message, uint32 flags)
{
	UINT type = 0;

	if (flags & POPUP_FLAG_BUTTON_OK)
	{
		type = MB_OK;
	}
	else if (flags & POPUP_FLAG_BUTTONS_CANCEL_TRY_CONTINUE)
	{
		type = MB_CANCELTRYCONTINUE;
	}
	else if (flags & POPUP_FLAG_BUTTONS_RETRY_CANCEL)
	{
		type = MB_RETRYCANCEL;
	}
	else if (flags & POPUP_FLAG_BUTTONS_YES_NO)
	{
		type = MB_YESNO;
	}
	else if (flags & POPUP_FLAG_BUTTONS_YES_NO_CANCEL)
	{
		type = MB_YESNOCANCEL;
	}
	else
	{
		HC_LOG_ERROR("Platform::OpenPopup - No valid combination of buttons was specified!");
		return POPUP_FLAG_NONE;
	}

	if (flags & POPUP_FLAG_ICON_ERROR)
	{
		type |= MB_ICONERROR;
	}

	int button = MessageBoxA(NULL, message, title, type);
	switch (button)
	{
		case IDABORT:    return POPUP_FLAG_BUTTON_ABORT;
		case IDCANCEL:   return POPUP_FLAG_BUTTON_CANCEL;
		case IDCONTINUE: return POPUP_FLAG_BUTTON_CONTINUE;
		case IDIGNORE:   return POPUP_FLAG_BUTTON_IGNORE;
		case IDNO:       return POPUP_FLAG_BUTTON_NO;
		case IDYES:      return POPUP_FLAG_BUTTON_YES;
		case IDOK:       return POPUP_FLAG_BUTTON_OK;
		case IDRETRY:    return POPUP_FLAG_BUTTON_RETRY;
		case IDTRYAGAIN: return POPUP_FLAG_BUTTON_TRY;
	}

	HC_LOG_ERROR("Platform::OpenPopup - No button was pressed?");
	return POPUP_FLAG_NONE;
}

} // namespace HC