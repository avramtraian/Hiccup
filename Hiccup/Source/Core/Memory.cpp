// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Memory.h"

#include <cstdlib>

namespace HC
{
	


} // namespace HC

void* operator new(size_t bytesCount)
{
	return std::malloc(bytesCount);
}

void* operator new(size_t bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber)
{
	return std::malloc(bytesCount);
}

void operator delete(void* memoryBlock)
{
	return std::free(memoryBlock);
}