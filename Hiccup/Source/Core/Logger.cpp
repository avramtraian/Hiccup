// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Logger.h"

#include "Platform/Platform.h"
#include "Memory/Memory.h"
#include "Memory/Buffer.h"
#include "Math/MathUtilities.h"

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

	Buffer FormatBuffer;
	Buffer LogBuffer;

#endif // HC_ENABLE_LOGS
};
static_internal LoggerData* s_LoggerData = nullptr;

bool Logger::Initialize(const LoggerSpecification& specification)
{
	s_LoggerData = hc_new LoggerData();

	s_LoggerData->Specification = specification;

	Buffer buffer = Buffer(2 * kilobytes(8));

	s_LoggerData->FormatBuffer.data = buffer.data;
	s_LoggerData->FormatBuffer.size = buffer.size / 2;

	s_LoggerData->LogBuffer.data = buffer.data + buffer.size / 2;
	s_LoggerData->LogBuffer.size = buffer.size / 2;

	return true;
}

void Logger::Shutdown()
{
	Buffer buffer;
	buffer.data = s_LoggerData->FormatBuffer.data;
	buffer.size = s_LoggerData->FormatBuffer.size + s_LoggerData->LogBuffer.size;
	buffer.release();

	hc_delete s_LoggerData;
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

	vsprintf_s(s_LoggerData->FormatBuffer.as<char>(), s_LoggerData->FormatBuffer.size, message, argList);

	Platform::SystemTime systemTime;
	Platform::GetLocalSystemTime(&systemTime);

	int logLength = sprintf_s(
		s_LoggerData->LogBuffer.as<char>(), s_LoggerData->LogBuffer.size,
		"[%02u:%02u:%02u][%s]%s %s\n",
		systemTime.Hour, systemTime.Minute, systemTime.Second,
		tag,
		s_LoggerData->TypeStrings[(uint8)type],
		s_LoggerData->FormatBuffer.as<const char>()
	);

	va_end(argList);

	Platform::SetConsoleColor(
		s_LoggerData->TypeColorFG[(uint8)type],
		s_LoggerData->TypeColorBG[(uint8)type]
	);
	Platform::WriteToConsole(s_LoggerData->LogBuffer.as<const char>(), (usize)logLength);
}

#endif // HC_ENABLE_LOGS

} // namespace HC