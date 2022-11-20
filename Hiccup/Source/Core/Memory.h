// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#define HcNew new(HC_FILE, HC_FUNCTION, HC_LINE)

#define HcDelete delete

#define AllocateTaggedI(BYTES_COUNT) AllocateTagged((BYTES_COUNT), HC_FILE, HC_FUNCTION, HC_LINE)

namespace HC
{



} // namespace HC

inline void* operator new(size_t bytesCount, void* memoryBlock)
{
	return memoryBlock;
}

void* operator new(size_t bytesCount);

void* operator new(size_t bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber);

void operator delete(void* memoryBlock);