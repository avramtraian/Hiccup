// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#include "Core/Memory/Memory.h"

void* operator new(size_t bytes_count)
{
	return HC::Memory::allocate(bytes_count);
}

void* operator new(size_t bytes_count, const char* filename, const char* function_sig, uint32_t line_number)
{
	return HC::Memory::allocate_tagged(bytes_count, filename, function_sig, line_number);
}

void operator delete(void* memory_block)
{
	HC::Memory::free(memory_block);
}