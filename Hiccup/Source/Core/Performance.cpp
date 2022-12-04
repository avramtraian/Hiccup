// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Performance.h"

#include "Memory.h"
#include "Platform/Platform.h"

namespace HC
{

#if HC_ENABLE_PROFILING

struct ProfilerData
{
	ProfilerDescription   description;
	uint64                frame_index;
	bool                  is_in_frame;
};
static_internal ProfilerData* s_profiler_data = nullptr;

bool Profiler::initializer(const ProfilerDescription& description)
{
	s_profiler_data = hc_new ProfilerData();

	s_profiler_data->description = description;
	s_profiler_data->frame_index = 0;
	s_profiler_data->is_in_frame = false;

	return true;
}

void Profiler::shutdown()
{
	hc_delete s_profiler_data;
	s_profiler_data = nullptr;
}

void Profiler::begin_frame()
{
	if (s_profiler_data->is_in_frame)
	{
		HC_LOG_WARN("Profiler::BeginFrame - Trying to begin a new frame, while another one is in flight!");
		return;
	}

	s_profiler_data->is_in_frame = true;
}

void Profiler::end_frame()
{
	if (!s_profiler_data->is_in_frame)
	{
		HC_LOG_WARN("Profiler::EndFrame - No frame is in flight.");
		return;
	}

	s_profiler_data->is_in_frame = false;
	s_profiler_data->frame_index++;
}

Profiler::ScopedTimer::ScopedTimer(const char* scope_name)
	: m_name(scope_name)
{
	m_entering_time = Platform::get_nanoseconds_since_initialization();
}

Profiler::ScopedTimer::~ScopedTimer()
{
	const uint64 exitingTime = Platform::get_nanoseconds_since_initialization();
	const uint64 scopeTime = exitingTime - m_entering_time;

	// s_ProfilerData->ScopedTimerRecords[m_Name] += scopeTime;
}

#endif // HC_ENABLE_PROFILING

} // namespace HC