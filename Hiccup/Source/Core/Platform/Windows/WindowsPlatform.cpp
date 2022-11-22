// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Platform/Platform.h"

#include "Core/Memory.h"

#include <Windows.h>
#include <cstdlib>

namespace HC
{

struct WindowsPlatformData
{
	PlatformSpecification Specification;

	HANDLE ConsoleHandle;
	Platform::ConsoleColor ConsoleForeground;
	Platform::ConsoleColor ConsoleBackground;
};
internal WindowsPlatformData* s_PlatformData = nullptr;

bool Platform::Initialize(const PlatformSpecification& specification)
{
	s_PlatformData = (WindowsPlatformData*)malloc(sizeof(WindowsPlatformData));
	if (!s_PlatformData)
	{
		return false;
	}

	new (s_PlatformData) WindowsPlatformData();

	s_PlatformData->Specification = specification;

	if (s_PlatformData->Specification.IsConsoleAttached)
	{
		s_PlatformData->ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		s_PlatformData->ConsoleForeground = ConsoleColor::MaxEnumValue;
		s_PlatformData->ConsoleBackground = ConsoleColor::MaxEnumValue;
		SetConsoleColor(ConsoleColor::LightGray, ConsoleColor::Black);
	}

	return true;
}

void Platform::Shutdown()
{
	if (s_PlatformData->Specification.IsConsoleAttached)
	{
		SetConsoleColor(ConsoleColor::LightGray, ConsoleColor::Black);
	}

	s_PlatformData->~WindowsPlatformData();
	free(s_PlatformData);
	s_PlatformData = nullptr;
}

void* Platform::AllocateMemory(usize bytesCount)
{
	return std::malloc(bytesCount);
}

void Platform::FreeMemory(void* memoryBlock)
{
	std::free(memoryBlock);
}

void Platform::SetConsoleColor(ConsoleColor foreground, ConsoleColor background)
{
	if (!s_PlatformData->Specification.IsConsoleAttached)
	{
		return;
	}

	if (s_PlatformData->ConsoleForeground == foreground && s_PlatformData->ConsoleBackground == background)
	{
		return;
	}

	s_PlatformData->ConsoleForeground = foreground;
	s_PlatformData->ConsoleBackground = background;

	const WORD consoleColor = (WORD)s_PlatformData->ConsoleForeground | ((WORD)s_PlatformData->ConsoleBackground << 4);
	SetConsoleTextAttribute(s_PlatformData->ConsoleHandle, consoleColor);
}

void Platform::WriteToConsole(const char* message, usize messageLength)
{
	if (!s_PlatformData->Specification.IsConsoleAttached)
	{
		return;
	}

	WriteConsoleA(s_PlatformData->ConsoleHandle, message, (DWORD)messageLength, NULL, NULL);
}

void Platform::GetLocalSystemTime(SystemTime* outSystemTime)
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	outSystemTime->Year = systemTime.wYear;
	outSystemTime->Month = systemTime.wMonth;
	outSystemTime->Day = systemTime.wDay;
	outSystemTime->Hour = systemTime.wHour;
	outSystemTime->Second = systemTime.wSecond;
	outSystemTime->Minute = systemTime.wMinute;
	outSystemTime->Millisecond = systemTime.wMilliseconds;
}

void Platform::GetGlobalSystemTime(SystemTime* outSystemTime)
{
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	outSystemTime->Year = systemTime.wYear;
	outSystemTime->Month = systemTime.wMonth;
	outSystemTime->Day = systemTime.wDay;
	outSystemTime->Hour = systemTime.wHour;
	outSystemTime->Second = systemTime.wSecond;
	outSystemTime->Minute = systemTime.wMinute;
	outSystemTime->Millisecond = systemTime.wMilliseconds;
}

uint32 Platform::OpenPopup(const char* title, const char* message, uint32 flags)
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