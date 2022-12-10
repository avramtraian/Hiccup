// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#include "Span.h"

namespace HC
{

//----------------------------------------------------------------
// Hiccup StringView. A view towards an UTF-8 encoded string.
//----------------------------------------------------------------
class HC_API StringView
{
public:
	ALWAYS_INLINE constexpr StringView() = default;

	ALWAYS_INLINE constexpr StringView(const StringView& other)
		: m_view(other.m_view)
	{}

	ALWAYS_INLINE constexpr StringView(StringView&& other) noexcept
		: m_view(Types::move(other.m_view))
	{}

	ALWAYS_INLINE constexpr StringView(const char* utf8_string, usize utf8_string_bytes_count)
		: m_view(utf8_string, utf8_string_bytes_count)
	{}

public:
	ALWAYS_INLINE StringView& operator=(const StringView& other)
	{
		m_view = other.m_view;
		return *this;
	}

	ALWAYS_INLINE StringView& operator=(StringView&& other) noexcept
	{
		m_view = Types::move(other.m_view);
		return *this;
	}

public:
	// Allows easy compatibility with C libraries (or any library that uses raw strings).
	ALWAYS_INLINE NODISCARD const char* c_str() const { return (const char*)bytes(); }

	ALWAYS_INLINE NODISCARD uint8* bytes() const { return m_view.bytes(); }

	ALWAYS_INLINE NODISCARD usize bytes_count() const { return m_view.bytes_count(); }

	// Wrapper around bytes() and bytes_count().
	ALWAYS_INLINE NODISCARD Span<uint8> bytes_span() const { return Span<uint8>(bytes(), bytes_count()); }

public:
	ALWAYS_INLINE NODISCARD Span<const char> to_span() const
	{
		return m_view;
	}
	
	static NODISCARD StringView from_span(Span<const char> utf8_span)
	{
		return StringView((const char*)utf8_span.bytes(), utf8_span.bytes_count());
	}

private:
	Span<const char> m_view;
};

} // namespace HC