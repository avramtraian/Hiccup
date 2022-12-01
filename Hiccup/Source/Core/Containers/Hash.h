// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

ALWAYS_INLINE uint64 ComputeHash(const uint8& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const uint16& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const uint32& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const uint64& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const int8& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const int16& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const int32& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const int64& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const char* const& value)
{
	return 0;
}

ALWAYS_INLINE uint64 ComputeHash(char* const& value)
{
	return ComputeHash((const char*)value);
}

ALWAYS_INLINE uint64 ComputeHash(const char& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 ComputeHash(const void* const& value)
{
	return (uint64)(uptr)value;
}

ALWAYS_INLINE uint64 ComputeHash(void* const& value)
{
	return ComputeHash((const void*)value);
}

/**
 *----------------------------------------------------------------
 * Hiccup Default Hasher.
 *----------------------------------------------------------------
 * Used, by default, in most containers where element hashing is required.
 */
class DefaultHasher
{
public:
	template<typename T>
	ALWAYS_INLINE static uint64 Compute(const T& object)
	{
		return ComputeHash(object);
	}
};

} // namespace HC