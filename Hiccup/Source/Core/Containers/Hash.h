// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

ALWAYS_INLINE uint64 compute_hash(const uint8& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const uint16& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const uint32& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const uint64& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const int8& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const int16& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const int32& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const int64& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const char* const& value)
{
	return 0;
}

ALWAYS_INLINE uint64 compute_hash(char* const& value)
{
	return compute_hash((const char*)value);
}

ALWAYS_INLINE uint64 compute_hash(const char& value)
{
	return (uint64)value;
}

ALWAYS_INLINE uint64 compute_hash(const void* const& value)
{
	return (uint64)(uptr)value;
}

ALWAYS_INLINE uint64 compute_hash(void* const& value)
{
	return compute_hash((const void*)value);
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
	ALWAYS_INLINE static uint64 compute(const T& object)
	{
		return compute_hash(object);
	}
};

} // namespace HC