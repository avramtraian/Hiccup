// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Platform/Platform.h"

#include "Core/Memory.h"

#include <cstdlib>

namespace HC
{

struct WindowsPlatformData
{
	PlatformSpecification Specification;
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

	return true;
}

void Platform::Shutdown()
{
	s_PlatformData->~WindowsPlatformData();
	free(s_PlatformData);
	s_PlatformData = nullptr;
}

void* Platform::Allocate(usize bytesCount)
{
	return std::malloc(bytesCount);
}

void Platform::Free(void* memoryBlock)
{
	std::free(memoryBlock);
}

} // namespace HC