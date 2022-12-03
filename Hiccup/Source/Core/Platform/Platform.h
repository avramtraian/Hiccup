// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Platform System Specification.
 *----------------------------------------------------------------
 */
struct PlatformSpecification
{
	// Whether or not the console is attached.
	bool IsConsoleAttached;
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
		Black    = 0,  Blue        = 1,  Green       = 2,  Aqua       = 3,
		Red      = 4,  Purple      = 5,  Yellow      = 6,  LightGray  = 7,
		Gray     = 8,  LightBlue   = 9,  LightGreen  = 10, LightAqua  = 11,
		LightRed = 12, LightPurple = 13, LightYellow = 14, White      = 15,

		MaxEnumValue
	};

	struct SystemTime
	{
		uint16 Year;
		uint16 Month;
		uint16 Day;
		uint16 Hour;
		uint16 Minute;
		uint16 Second;
		uint16 Millisecond;
	};

	enum PopupFlagsEnum : uint32
	{
		POPUP_FLAG_NONE             = 0,

		POPUP_FLAG_BUTTON_OK        = Bit(0),
		POPUP_FLAG_BUTTON_ABORT     = Bit(1),
		POPUP_FLAG_BUTTON_CLOSE     = Bit(2),
		POPUP_FLAG_BUTTON_CANCEL    = Bit(3),
		POPUP_FLAG_BUTTON_RETRY     = Bit(4),
		POPUP_FLAG_BUTTON_TRY       = Bit(5),
		POPUP_FLAG_BUTTON_CONTINUE  = Bit(6),
		POPUP_FLAG_BUTTON_IGNORE    = Bit(7),
		POPUP_FLAG_BUTTON_YES       = Bit(8),
		POPUP_FLAG_BUTTON_NO        = Bit(9),

		POPUP_FLAG_BUTTONS_ABORT_RETRY_IGNORE  = POPUP_FLAG_BUTTON_ABORT  | POPUP_FLAG_BUTTON_RETRY | POPUP_FLAG_BUTTON_IGNORE,
		POPUP_FLAG_BUTTONS_CANCEL_TRY_CONTINUE = POPUP_FLAG_BUTTON_CANCEL | POPUP_FLAG_BUTTON_TRY   | POPUP_FLAG_BUTTON_CONTINUE,
		POPUP_FLAG_BUTTONS_OK_CANCEL           = POPUP_FLAG_BUTTON_OK     | POPUP_FLAG_BUTTON_CANCEL,
		POPUP_FLAG_BUTTONS_RETRY_CANCEL        = POPUP_FLAG_BUTTON_RETRY  | POPUP_FLAG_BUTTON_CANCEL,
		POPUP_FLAG_BUTTONS_YES_NO              = POPUP_FLAG_BUTTON_YES    | POPUP_FLAG_BUTTON_NO,
		POPUP_FLAG_BUTTONS_YES_NO_CANCEL       = POPUP_FLAG_BUTTON_YES    | POPUP_FLAG_BUTTON_NO    | POPUP_FLAG_BUTTON_CANCEL,

		POPUP_FLAG_ICON_ERROR       = Bit(10)
	};

public:
	static bool Initialize(const PlatformSpecification& specification);
	static void Shutdown();

public:
	static void* AllocateMemory(usize bytesCount);

	static void FreeMemory(void* memoryBlock);

public:
	static uint64 GetPerformanceTickCount();
	static uint64 GetPerformanceTickFrequency();

	static uint64 GetNanoseconds();
	static uint64 GetNanosecondsSinceInitialization();

public:
	static void SetConsoleColor(ConsoleColor foreground, ConsoleColor background);

	static void WriteToConsole(const char* message, usize messageLength);

public:
	static void GetLocalSystemTime(SystemTime* outSystemTime);
	static void GetGlobalSystemTime(SystemTime* outSystemTime);

public:
	static uint32 OpenPopup(const char* title, const char* message, uint32 flags);
};

} // namespace HC