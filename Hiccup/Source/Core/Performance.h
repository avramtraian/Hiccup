// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#define HC_ENABLE_PROFILING 1

#if HC_ENABLE_PROFILING
	#define HC_PROFILE_INTERNAL1(SCOPE_NAME, LINE)  ::HC::Profiler::ScopedTimer __timer##LINE##(SCOPE_NAME)
	#define HC_PROFILE_INTERNAL2(SCOPE_NAME, LINE)  HC_PROFILE_INTERNAL1(SCOPE_NAME, LINE)

	#define HC_PROFILE_FUNCTION()                   HC_PROFILE_INTERNAL2(HC_FUNCTION_NAME, HC_LINE)
	#define HC_PROFILE_SCOPE(SCOPE_NAME)            HC_PROFILE_INTERNAL2(SCOPE_NAME, HC_LINE)

	#define HC_PROFILE_BEGIN_FRAME                  ::HC::Profiler::BeginFrame()
	#define HC_PROFILE_END_FRAME                    ::HC::Profiler::EndFrame()
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
 * Hiccup Performance Profiler Tool Specification.
 *----------------------------------------------------------------
*/
struct ProfilerSpecification
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
	static bool Initialize(const ProfilerSpecification& specification);
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

public:
	struct ScopedTimer
	{
	public:
		ScopedTimer(const char* scopeName);
		~ScopedTimer();

	private:
		const char* m_Name;
		uint64 m_EnteringTime;
	};
};

#endif // HC_ENABLE_PROFILING

} // namespace HC