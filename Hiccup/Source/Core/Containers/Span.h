// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"

#include <initializer_list>

namespace HC
{

template<typename T>
class Span
{
public:
	ALWAYS_INLINE constexpr Span()
		: m_elements(nullptr)
		, m_count(0)
	{}

	ALWAYS_INLINE constexpr Span(const Span<T>& other)
		: m_elements(other.m_elements)
		, m_count(other.m_count)
	{}

	ALWAYS_INLINE constexpr Span(Span<T>&& other) noexcept
		: m_elements(other.m_elements)
		, m_count(other.m_count)
	{
		other.m_elements = nullptr;
		other.m_count = 0;
	}

	ALWAYS_INLINE constexpr Span(T* values, size_t count)
		: m_elements(values)
		, m_count(count)
	{}

	template<size_t Count>
	ALWAYS_INLINE constexpr Span(T (&values)[Count])
		: m_elements(values)
		, m_count(Count)
	{}

	ALWAYS_INLINE constexpr Span(std::initializer_list<T> list)
		: m_elements(list.begin())
		, m_count(list.size())
	{}

public:
	ALWAYS_INLINE constexpr Span<T>& operator=(const Span<T>& other)
	{
		m_elements = other.m_elements;
		m_count = other.m_count;
		return *this;
	}

	ALWAYS_INLINE constexpr Span<T>& operator=(Span<T>&& other) noexcept
	{
		m_elements = other.m_elements;
		m_count = other.m_count;
		other.m_elements = nullptr;
		other.m_count = 0;
		return *this;
	}

	template<size_t Count>
	ALWAYS_INLINE constexpr Span<T>& operator=(T(&values)[Count])
	{
		m_elements = values;
		m_count = Count;
		return *this;
	}

	ALWAYS_INLINE constexpr Span<T>& operator=(std::initializer_list<T> list)
	{
		m_elements = list.begin();
		m_count = list.size();
		return *this;
	}

public:
	// Returns a pointer to the first elements that the span ranges over.
	// The const-ness of the pointer is the same as the function's.
	ALWAYS_INLINE T* elements() { return m_elements; }
	ALWAYS_INLINE const T* elements() const { return m_elements; }

	// Returns the number of elements the span ranges over.
	ALWAYS_INLINE size_t count() const { return m_count; }

	// Checks whether or not the span is empty (has no elements).
	ALWAYS_INLINE bool is_empty() const { return (m_count == 0); }

	// Returns a pointer to the first byte that the span ranges over.
	// The read/write modifiers of the bytes are given by the const-ness of the function.
	ALWAYS_INLINE ReadonlyBytes bytes() const { return (ReadonlyBytes)m_elements; }
	ALWAYS_INLINE ReadWriteBytes bytes() { return (ReadWriteBytes)m_elements; }

	// Wrappers around 'bytes', explicitly specifying the bytes' read/write modifiers.
	ALWAYS_INLINE ReadonlyBytes readonly_bytes() const { return bytes(); }
	ALWAYS_INLINE WriteonlyBytes writeonly_bytes() { return bytes(); }
	ALWAYS_INLINE ReadWriteBytes read_write_bytes() { return bytes(); }

	// Returns the number of bytes that the span ranges over.
	ALWAYS_INLINE size_t bytes_count() const { return m_count * sizeof(T); }

private:
	// Pointer to the first elements the span ranges over.
	T* m_elements;
	// The number of elements the span ranges over.
	size_t m_count;
};

} // namespace HC
