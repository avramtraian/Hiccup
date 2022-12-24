// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

namespace HC
{

ALWAYS_INLINE uint64_t compute_hash(const uint8_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const uint16_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const uint32_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const uint64_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const int8_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const int16_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const int32_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const int64_t& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const char* const& value)
{
	return 0;
}

ALWAYS_INLINE uint64_t compute_hash(char* const& value)
{
	return compute_hash((const char*)value);
}

ALWAYS_INLINE uint64_t compute_hash(const char& value)
{
	return (uint64_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(const void* const& value)
{
	return (uint64_t)(uintptr_t)value;
}

ALWAYS_INLINE uint64_t compute_hash(void* const& value)
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
	ALWAYS_INLINE static uint64_t compute(const T& object)
	{
		return compute_hash(object);
	}
};

} // namespace HC