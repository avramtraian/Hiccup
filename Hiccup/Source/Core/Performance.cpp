// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Performance.h"

#include "Memory.h"
#include "Platform/Platform.h"

namespace HC
{

#if HC_ENABLE_PROFILING

struct ProfilerData
{
	ProfilerSpecification   Specification = {};
	uint64                  FrameIndex    = 0;
	bool                    InFrame       = false;
};
static_internal ProfilerData* s_ProfilerData = nullptr;

bool Profiler::Initialize(const ProfilerSpecification& specification)
{
	s_ProfilerData = hc_new ProfilerData();
	s_ProfilerData->Specification = specification;

	return true;
}

void Profiler::Shutdown()
{
	hc_delete s_ProfilerData;
	s_ProfilerData = nullptr;
}

void Profiler::BeginFrame()
{
	if (s_ProfilerData->InFrame)
	{
		HC_LOG_WARN("Profiler::BeginFrame - Trying to begin a new frame, while another one is in flight!");
		return;
	}

	s_ProfilerData->InFrame = true;
}

void Profiler::EndFrame()
{
	if (!s_ProfilerData->InFrame)
	{
		HC_LOG_WARN("Profiler::EndFrame - No frame is in flight.");
		return;
	}

	s_ProfilerData->InFrame = false;
	s_ProfilerData->FrameIndex++;
}

Profiler::ScopedTimer::ScopedTimer(const char* scopeName)
	: m_Name(scopeName)
{
	m_EnteringTime = Platform::GetNanosecondsSinceInitialization();
}

Profiler::ScopedTimer::~ScopedTimer()
{
	const uint64 exitingTime = Platform::GetNanosecondsSinceInitialization();
	const uint64 scopeTime = exitingTime - m_EnteringTime;

	// s_ProfilerData->ScopedTimerRecords[m_Name] += scopeTime;
}

#endif // HC_ENABLE_PROFILING

} // namespace HC