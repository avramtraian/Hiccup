// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Logger.h"

#include "Memory.h"
#include "Platform/Platform.h"

#include <cstdarg>
#include <cstdio>

namespace HC
{

struct LoggerData
{
	LoggerSpecification Specification;

	const char* TypeStrings[(uint8)Logger::LogType::MaxEnumValue] =
	{
		"[DEBUG]:",
		"[TRACE]:",
		"[INFO]: ",
		"[WARN]: ",
		"[ERROR]:",
		"[FATAL]:"
	};

	Platform::ConsoleColor TypeColorFG[(uint8)Logger::LogType::MaxEnumValue] =
	{
		Platform::ConsoleColor::Purple,         // DEBUG
		Platform::ConsoleColor::Gray,           // TRACE
		Platform::ConsoleColor::Green,          // INFO
		Platform::ConsoleColor::LightYellow,    // WARN
		Platform::ConsoleColor::LightRed,       // ERROR
		Platform::ConsoleColor::White           // FATAL
	};

	Platform::ConsoleColor TypeColorBG[(uint8)Logger::LogType::MaxEnumValue] =
	{
		Platform::ConsoleColor::Black,  // DEBUG
		Platform::ConsoleColor::Black,  // TRACE
		Platform::ConsoleColor::Black,  // INFO
		Platform::ConsoleColor::Black,  // WARN
		Platform::ConsoleColor::Black,  // ERROR
		Platform::ConsoleColor::Red     // FATAL
	};

#if HC_ENABLE_LOGS

	// TODO(Traian): Don't imply a limit.
	char FormatBuffer[8192];
	char LogBuffer[8192];

#endif // HC_ENABLE_LOGS
};
internal LoggerData* s_LoggerData = nullptr;

bool Logger::Initialize(const LoggerSpecification& specification)
{
	s_LoggerData = HcNew LoggerData();

	s_LoggerData->Specification = specification;

	return true;
}

void Logger::Shutdown()
{
	HcDelete s_LoggerData;
	s_LoggerData = nullptr;
}

#if HC_ENABLE_LOGS

void Logger::Log(LogType type, const char* tag, const char* message, ...)
{
	if (!s_LoggerData)
	{
		return;
	}

	va_list argList;
	va_start(argList, message);

	vsprintf_s(s_LoggerData->FormatBuffer, message, argList);

	Platform::SystemTime systemTime;
	Platform::GetLocalSystemTime(&systemTime);

	int logLength = sprintf_s(
		s_LoggerData->LogBuffer, "[%02u:%02u:%02u][%s]%s %s\n",
		systemTime.Hour, systemTime.Minute, systemTime.Second,
		tag,
		s_LoggerData->TypeStrings[(uint8)type],
		s_LoggerData->FormatBuffer
	);

	va_end(argList);

	Platform::SetConsoleColor(
		s_LoggerData->TypeColorFG[(uint8)type],
		s_LoggerData->TypeColorBG[(uint8)type]
	);
	Platform::WriteToConsole(s_LoggerData->LogBuffer, logLength);
}

#endif // HC_ENABLE_LOGS

} // namespace HC