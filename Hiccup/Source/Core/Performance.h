// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#define HC_ENABLE_PROFILING 1

#if HC_ENABLE_PROFILING
	#define HC_PROFILE_INTERNAL1(SCOPE_NAME, LINE)  ::HC::Profiler::ScopedTimer __timer##LINE##(SCOPE_NAME)
	#define HC_PROFILE_INTERNAL2(SCOPE_NAME, LINE)  HC_PROFILE_INTERNAL1(SCOPE_NAME, LINE)

	#define HC_PROFILE_FUNCTION()                   HC_PROFILE_INTERNAL2(HC_FUNCTION_NAME, HC_LINE)
	#define HC_PROFILE_SCOPE(SCOPE_NAME)            HC_PROFILE_INTERNAL2(SCOPE_NAME, HC_LINE)

	#define HC_PROFILE_BEGIN_FRAME                  ::HC::Profiler::begin_frame()
	#define HC_PROFILE_END_FRAME                    ::HC::Profiler::end_frame()
#else
	#define HC_PROFILE_FUNCTION()
	#define HC_PROFILE_SCOPE(SCOPE_NAME)

	#define HC_PROFILE_END_FRAME
	#define HC_PROFILE_BEGIN_FRAME
#endif // HC_ENABLE_PROFILING

namespace HC
{

#if HC_ENABLE_PROFILING

/**
 *----------------------------------------------------------------
 * Hiccup Performance Profiler Tool Description.
 *----------------------------------------------------------------
 */
struct ProfilerDescription
{
};

/**
 *----------------------------------------------------------------
 * Hiccup Performance Profiler Tool.
 *----------------------------------------------------------------
 * This class holds all the functionality that the Hiccup Performance Profiler Tool API provides.
 */
class Profiler
{
public:
	static bool initializer(const ProfilerDescription& description);
	static void shutdown();

	static void begin_frame();
	static void end_frame();

public:
	struct ScopedTimer
	{
	public:
		ScopedTimer(const char* scope_name);
		~ScopedTimer();

	private:
		const char* m_name;
		uint64 m_entering_time;
	};
};

#endif // HC_ENABLE_PROFILING

} // namespace HC