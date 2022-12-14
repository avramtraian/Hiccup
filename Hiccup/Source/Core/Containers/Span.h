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
		: m_values(nullptr)
		, m_count(0)
	{}

	ALWAYS_INLINE constexpr Span(const Span<T>& other)
		: m_values(other.m_values)
		, m_count(other.m_count)
	{}

	ALWAYS_INLINE constexpr Span(Span<T>&& other) noexcept
		: m_values(other.m_values)
		, m_count(other.m_count)
	{
		other.m_values = nullptr;
		other.m_count = 0;
	}

	ALWAYS_INLINE constexpr Span(T* values, usize count)
		: m_values(values)
		, m_count(count)
	{}

	template<usize Count>
	ALWAYS_INLINE constexpr Span(T (&values)[Count])
		: m_values(values)
		, m_count(Count)
	{}

	ALWAYS_INLINE constexpr Span(std::initializer_list<T> list)
		: m_values(list.begin())
		, m_count(list.size())
	{}

public:
	ALWAYS_INLINE constexpr Span<T>& operator=(const Span<T>& other)
	{
		m_values = other.m_values;
		m_count = other.m_count;
		return *this;
	}

	ALWAYS_INLINE constexpr Span<T>& operator=(Span<T>&& other) noexcept
	{
		m_values = other.m_values;
		m_count = other.m_count;
		other.m_values = nullptr;
		other.m_count = 0;
		return *this;
	}

	template<usize Count>
	ALWAYS_INLINE constexpr Span<T>& operator=(T(&values)[Count])
	{
		m_values = values;
		m_count = Count;
		return *this;
	}

	ALWAYS_INLINE constexpr Span<T>& operator=(std::initializer_list<T> list)
	{
		m_values = list.begin();
		m_count = list.size();
		return *this;
	}

public:
	ALWAYS_INLINE T* values() { return m_values; }
	ALWAYS_INLINE const T* values() const { return m_values; }

	ALWAYS_INLINE usize count() const { return m_count; }

	ALWAYS_INLINE bool is_empty() const { return (m_count == 0); }

	ALWAYS_INLINE uint8* bytes() const { return (uint8*)m_values; }

	ALWAYS_INLINE usize bytes_count() const { return m_count * sizeof(T); }

private:
	T* m_values;
	usize m_count;
};

} // namespace HC