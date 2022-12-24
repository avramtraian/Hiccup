// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Platform System Description.
 *----------------------------------------------------------------
 */
struct PlatformDescription
{
	// Whether or not the console is attached.
	bool is_console_attached;
};

/**
 *----------------------------------------------------------------
 * Hiccup Platform System.
 *----------------------------------------------------------------
 * This class holds all the functionality that the Hiccup Platform API provides.
 * Usually, these calls should only be used internally by other higher-level systems.
 */
class Platform
{
public:
	// All available colors for the console text.
	enum class ConsoleColor
	{
		Black     = 0, Blue        = 1,  Green       = 2,  Aqua      = 3,
		Red       = 4, Purple      = 5,  Yellow      = 6,  LightGray = 7,
		Gray      = 8, LightBlue   = 9,  LightGreen  = 10, LightAqua = 11,
		LightRed = 12, LightPurple = 13, LightYellow = 14, White     = 15,

		MaxEnumValue
	};

	struct SystemTime
	{
		uint16_t year;
		uint16_t month;
		uint16_t day;
		uint16_t hour;
		uint16_t minute;
		uint16_t second;
		uint16_t millisecond;
	};

	enum PopupFlagsEnum : uint32_t
	{
		POPUP_FLAG_NONE             = 0,

		POPUP_FLAG_BUTTON_OK        = bit(0),
		POPUP_FLAG_BUTTON_ABORT     = bit(1),
		POPUP_FLAG_BUTTON_CLOSE     = bit(2),
		POPUP_FLAG_BUTTON_CANCEL    = bit(3),
		POPUP_FLAG_BUTTON_RETRY     = bit(4),
		POPUP_FLAG_BUTTON_TRY       = bit(5),
		POPUP_FLAG_BUTTON_CONTINUE  = bit(6),
		POPUP_FLAG_BUTTON_IGNORE    = bit(7),
		POPUP_FLAG_BUTTON_YES       = bit(8),
		POPUP_FLAG_BUTTON_NO        = bit(9),

		POPUP_FLAG_BUTTONS_ABORT_RETRY_IGNORE  = POPUP_FLAG_BUTTON_ABORT  | POPUP_FLAG_BUTTON_RETRY | POPUP_FLAG_BUTTON_IGNORE,
		POPUP_FLAG_BUTTONS_CANCEL_TRY_CONTINUE = POPUP_FLAG_BUTTON_CANCEL | POPUP_FLAG_BUTTON_TRY   | POPUP_FLAG_BUTTON_CONTINUE,
		POPUP_FLAG_BUTTONS_OK_CANCEL           = POPUP_FLAG_BUTTON_OK     | POPUP_FLAG_BUTTON_CANCEL,
		POPUP_FLAG_BUTTONS_RETRY_CANCEL        = POPUP_FLAG_BUTTON_RETRY  | POPUP_FLAG_BUTTON_CANCEL,
		POPUP_FLAG_BUTTONS_YES_NO              = POPUP_FLAG_BUTTON_YES    | POPUP_FLAG_BUTTON_NO,
		POPUP_FLAG_BUTTONS_YES_NO_CANCEL       = POPUP_FLAG_BUTTON_YES    | POPUP_FLAG_BUTTON_NO    | POPUP_FLAG_BUTTON_CANCEL,

		POPUP_FLAG_ICON_ERROR       = bit(10)
	};

public:
	static bool initialize(const PlatformDescription& description);
	static void shutdown();

public:
	static void* allocate_memory(size_t bytes_count);

	static void free_memory(void* memory_block);

public:
	static uint64_t get_performance_tick_count();
	static uint64_t get_performance_tick_frequency();

	static uint64_t get_nanoseconds();
	static uint64_t get_nanoseconds_since_initialization();

public:
	static void set_console_color(ConsoleColor foreground, ConsoleColor background);

	static void write_to_console(const char* message, size_t message_length);

public:
	static void get_local_system_time(SystemTime* out_system_time);
	static void get_global_system_time(SystemTime* out_system_time);

public:
	static uint32_t open_popup(const char* title, const char* message, uint32_t flags);
};

} // namespace HC