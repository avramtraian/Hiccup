// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreDefines.h"
#include "CoreTypes.h"

#if HC_CONFIGURATION_DEBUG
	#define HC_ENABLE_DEBUG_LOGS            1
	#define HC_ENABLE_TRACE_LOGS            1
	#define HC_ENABLE_INFO_LOGS             1
	#define HC_ENABLE_WARN_LOGS             1
	#define HC_ENABLE_ERROR_LOGS            1
	#define HC_ENABLE_FATAL_LOGS            1
#elif HC_CONFIGURATION_RELEASE
	#define HC_ENABLE_DEBUG_LOGS            0
	#define HC_ENABLE_TRACE_LOGS            0
	#define HC_ENABLE_INFO_LOGS             1
	#define HC_ENABLE_WARN_LOGS             1
	#define HC_ENABLE_ERROR_LOGS            1
	#define HC_ENABLE_FATAL_LOGS            1
#elif HC_CONFIGURATION_SHIPPING
	#define HC_ENABLE_DEBUG_LOGS            0
	#define HC_ENABLE_TRACE_LOGS            0
	#define HC_ENABLE_INFO_LOGS             0
	#define HC_ENABLE_WARN_LOGS             0
	#define HC_ENABLE_ERROR_LOGS            0
	#define HC_ENABLE_FATAL_LOGS            0
#endif // Build configuration switch.

#if HC_ENABLE_DEBUG_LOGS || HC_ENABLE_TRACE_LOGS || \
    HC_ENABLE_INFO_LOGS  || HC_ENABLE_WARN_LOGS  || \
    HC_ENABLE_ERROR_LOGS || HC_ENABLE_FATAL_LOGS
	#define HC_ENABLE_LOGS                  1
#else
	#define HC_ENABLE_LOGS                  0
#endif // Any logs enabled.

namespace HC
{

/**
 *----------------------------------------------------------------
 * Logging System Description.
 *----------------------------------------------------------------
 */
struct LoggerDescription
{
};

/**
 *----------------------------------------------------------------
 * Hiccup Logging System.
 *----------------------------------------------------------------
 * This class holds all the functionality that the Hiccup Logging API provides.
 */
class Logger
{
public:
	// All types that a log can be.
	enum class LogType : uint8
	{
		debug = 0, trace = 1, info = 2, warn = 3, error = 4, fatal = 5,

		max_enum_value
	};

public:
	static bool initialize(const LoggerDescription& description);
	static void shutdown();

#if HC_ENABLE_LOGS

public:
	HC_API static void log(LogType type, const char* tag, const char* message, ...);

#endif // HC_ENABLE_LOGS
};

} // namespace HC

// Debug Logs.
#if HC_ENABLE_DEBUG_LOGS
	#define HC_LOG_DEBUG(...)           ::HC::Logger::log(::HC::Logger::LogType::debug, "CORE", __VA_ARGS__)
	#define HC_LOG_DEBUG_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::debug, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_DEBUG(...)
	#define HC_LOG_DEBUG_TAG(TAG, ...)
#endif // HC_ENABLE_DEBUG_LOGS

// Trace Logs.
#if HC_ENABLE_TRACE_LOGS
	#define HC_LOG_TRACE(...)           ::HC::Logger::log(::HC::Logger::LogType::trace, "CORE", __VA_ARGS__)
	#define HC_LOG_TRACE_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::trace, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_TRACE(...)
	#define HC_LOG_TRACE_TAG(TAG, ...)
#endif // HC_ENABLE_TRACE_LOGS

// Info Logs.
#if HC_ENABLE_INFO_LOGS
	#define HC_LOG_INFO(...)           ::HC::Logger::log(::HC::Logger::LogType::info, "CORE", __VA_ARGS__)
	#define HC_LOG_INFO_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::info, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_INFO(...)
	#define HC_LOG_INFO_TAG(TAG, ...)
#endif // HC_ENABLE_TRACE_LOGS

// Warn Logs.
#if HC_ENABLE_WARN_LOGS
	#define HC_LOG_WARN(...)           ::HC::Logger::log(::HC::Logger::LogType::warn, "CORE", __VA_ARGS__)
	#define HC_LOG_WARN_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::warn, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_WARN(...)
	#define HC_LOG_WARN_TAG(TAG, ...)
#endif // HC_ENABLE_TRACE_LOGS

// Error Logs.
#if HC_ENABLE_ERROR_LOGS
	#define HC_LOG_ERROR(...)           ::HC::Logger::log(::HC::Logger::LogType::error, "CORE", __VA_ARGS__)
	#define HC_LOG_ERROR_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::error, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_ERROR(...)
	#define HC_LOG_ERROR_TAG(TAG, ...)
#endif // HC_ENABLE_TRACE_LOGS

// Fatal Logs.
#if HC_ENABLE_FATAL_LOGS
	#define HC_LOG_FATAL(...)           ::HC::Logger::log(::HC::Logger::LogType::fatal, "CORE", __VA_ARGS__)
	#define HC_LOG_FATAL_TAG(TAG, ...)  ::HC::Logger::log(::HC::Logger::LogType::fatal, TAG,    __VA_ARGS__)
#else
	#define HC_LOG_FATAL(...)
	#define HC_LOG_FATAL_TAG(TAG, ...)
#endif // HC_ENABLE_TRACE_LOGS