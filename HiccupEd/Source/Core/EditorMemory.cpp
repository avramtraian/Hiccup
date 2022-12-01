// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Core/Memory/Memory.h"

void* operator new(size_t bytesCount)
{
	return HC::Memory::Allocate(bytesCount);
}

void* operator new(size_t bytesCount, const char* fileName, const char* functionName, HC::uint32 lineNumber)
{
	return HC::Memory::AllocateTagged(bytesCount, fileName, functionName, lineNumber);
}

void operator delete(void* memoryBlock)
{
	HC::Memory::Free(memoryBlock);
}