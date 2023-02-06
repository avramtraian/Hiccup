// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

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
    LoggerDescription description;

    const char* log_type_strings[(uint8_t)Logger::LogType::MaxEnumValue] =
    {
        "[DEBUG]:",
        "[TRACE]:",
        "[INFO]: ",
        "[WARN]: ",
        "[ERROR]:",
        "[FATAL]:"
    };

    Platform::ConsoleColor log_type_color_FG[(uint8_t)Logger::LogType::MaxEnumValue] =
    {
        Platform::ConsoleColor::Purple,         // DEBUG
        Platform::ConsoleColor::Gray,           // TRACE
        Platform::ConsoleColor::Green,          // INFO
        Platform::ConsoleColor::LightYellow,   // WARN
        Platform::ConsoleColor::LightRed,      // ERROR
        Platform::ConsoleColor::White           // FATAL
    };

    Platform::ConsoleColor log_type_color_BG[(uint8_t)Logger::LogType::MaxEnumValue] =
    {
        Platform::ConsoleColor::Black,  // DEBUG
        Platform::ConsoleColor::Black,  // TRACE
        Platform::ConsoleColor::Black,  // INFO
        Platform::ConsoleColor::Black,  // WARN
        Platform::ConsoleColor::Black,  // ERROR
        Platform::ConsoleColor::Red     // FATAL
    };

#if HC_ENABLE_LOGS

    Buffer format_buffer;
    Buffer log_buffer;

#endif // HC_ENABLE_LOGS
};
static_internal LoggerData* s_logger_data = nullptr;

bool Logger::initialize(const LoggerDescription& description)
{
    s_logger_data = hc_new LoggerData();

    s_logger_data->description = description;

    Buffer buffer = Buffer(2 * kilobytes(8));

#if HC_ENABLE_LOGS
    s_logger_data->format_buffer.data = buffer.data;
    s_logger_data->format_buffer.size = buffer.size / 2;

    s_logger_data->log_buffer.data = buffer.data + buffer.size / 2;
    s_logger_data->log_buffer.size = buffer.size / 2;
#endif

    return true;
}

void Logger::shutdown()
{
    Buffer buffer;
#if HC_ENABLE_LOGS
    buffer.data = s_logger_data->format_buffer.data;
    buffer.size = s_logger_data->format_buffer.size + s_logger_data->log_buffer.size;
#endif
    buffer.release();

    hc_delete s_logger_data;
    s_logger_data = nullptr;
}

#if HC_ENABLE_LOGS

void Logger::log(LogType type, const char* tag, const char* message, ...)
{
    if (!s_logger_data)
    {
        return;
    }

    va_list argList;
    va_start(argList, message);

    vsprintf_s(s_logger_data->format_buffer.as<char>(), s_logger_data->format_buffer.size, message, argList);

    Platform::SystemTime systemTime;
    Platform::get_local_system_time(&systemTime);

    int logLength = sprintf_s(
        s_logger_data->log_buffer.as<char>(), s_logger_data->log_buffer.size,
        "[%02u:%02u:%02u][%s]%s %s\n",
        systemTime.hour, systemTime.minute, systemTime.second,
        tag,
        s_logger_data->log_type_strings[(uint8_t)type],
        s_logger_data->format_buffer.as<const char>()
    );

    va_end(argList);

    Platform::set_console_color(
        s_logger_data->log_type_color_FG[(uint8_t)type],
        s_logger_data->log_type_color_BG[(uint8_t)type]
    );
    Platform::write_to_console(s_logger_data->log_buffer.as<const char>(), (size_t)logLength);
}

#endif // HC_ENABLE_LOGS

} // namespace HC