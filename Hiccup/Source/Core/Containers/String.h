// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#include "Span.h"

namespace HC
{

// Gets the number of bytes an UTF-8 encoded, null-terminated string occupies.
// The value returned doesn't include the null-termination byte/character.
HC_API NODISCARD usize utf8_string_bytes_count(const char* utf8_string);

// Gets the number of codepoints an UTF-8 encoded, null-terminated string has.
// The value returned doesn't include the null-termination character.
HC_API NODISCARD usize utf8_string_length(const char* utf8_string);

// Checks if two UTF-8 encoded, null-terminated strings are equal.
// Equality means that the strings are composed from the exact same sequence of characters,
//   sequence that also contains the null-termination character.
HC_API NODISCARD bool utf8_string_equals(const char* utf8_string_a, const char* utf8_string_b);

// Literal operator, allowing us to determine the size of an ASCII string literal at compile time.
// This is recommended to be used whenever we mix ASCII string literals and the String class.
ALWAYS_INLINE Span<const char> constexpr operator""sv(const char* ascii_string, std::size_t ascii_string_length)
{
	return Span<const char>(ascii_string, ascii_string_length);
}

//----------------------------------------------------------------
// Hiccup String. An UTF-8 encoded, null-terminated string.
//----------------------------------------------------------------
class HC_API String
{
private:
	constexpr static usize SSOSize = sizeof(char*);

public:
	String()
		: m_bytes_count(1)
	{
		m_sso_bytes[0] = 0;
	}

	String(const String& other)
		: m_bytes_count(0)
	{
		// This function expects m_bytes_count to be 0.
		construct_from_span(other.span());
	}

	String(String&& other) noexcept
		: m_bytes_count(other.m_bytes_count)

		// No matter where other's buffer is actually stored (heap or inlined), this
		//   will copy the data.
		// In case that the buffer is inlined, this is equivalent to calling Memory::copy,
		//   because m_heap_bytes and the inline buffer are stored in an union.
		// In case that the buffer is stored on heap, this is obviously correct.
		, m_heap_bytes(other.m_heap_bytes)
	{
		// Reset to the default state.
		other.m_bytes_count = 1;
		other.m_sso_bytes[0] = 0;
	}

	// Constructs a string from an UTF-8 null-terminated raw string.
	String(const char* utf8_string)
		: m_bytes_count(0)
	{
		// This function expects m_bytes_count to be 0.
		construct_from_span(Span<const char>(utf8_string, utf8_string_bytes_count(utf8_string)));
	}

	// Constructs a string from a span containing an UTF-8 encoded character sequence.
	// The bytes_count of the span shouldn't include the null-termination byte/character.
	String(Span<const char> utf8_span)
		: m_bytes_count(0)
	{
		// This function expects m_bytes_count to be 0.
		construct_from_span(utf8_span);
	}

	~String()
	{
		release_memory_check();
	}

public:
	String& operator=(const String& other)
	{
		assign_from_span(other.span());
		return *this;
	}

	String& operator=(String&& other) noexcept
	{
		release_memory_check();

		m_bytes_count = other.m_bytes_count;
		// See String::String(String&&) to see why this is correct.
		m_heap_bytes = other.m_heap_bytes;

		// Reset to the default state.
		other.m_bytes_count = 1;
		other.m_sso_bytes[0] = 0;

		return *this;
	}

	String& operator=(const char* utf8_string)
	{
		assign_from_span(Span<const char>(utf8_string, utf8_string_bytes_count(utf8_string)));
		return *this;
	}

	String& operator=(Span<const char> utf8_span)
	{
		assign_from_span(utf8_span);
		return *this;
	}

public:
	// Constructs a span of characters from the String's buffer.
	// It is a wrapper around bytes() and bytes_count().
	ALWAYS_INLINE NODISCARD Span<const char> span() const { return Span<const char>((const char*)bytes(), bytes_count()); }

	// Gets a pointer to the String's buffer.
	// This checks to see if the sso inlined buffer or the heap buffer is being used.
	// So, if you already know where the data is stored, it is recommended that you
	//   directly use m_heap_bytes or m_sso_bytes.
	ALWAYS_INLINE NODISCARD uint8* bytes() const { return (uint8*)(is_using_sso() ? m_sso_bytes : m_heap_bytes); }

	// Gets bytes_count.
	// Should only be called by the end-user. For internal purposes, always use m_bytes_count.
	ALWAYS_INLINE NODISCARD usize bytes_count() const { return m_bytes_count; }

	// Constructs a span from the String's buffer.
	// It is a wrapper around bytes() and bytes_count().
	ALWAYS_INLINE NODISCARD Span<uint8> bytes_span() const { return Span<uint8>(bytes(), bytes_count()); }

	// Allows easy compatibility with C libraries (or any library that uses raw strings).
	ALWAYS_INLINE NODISCARD const char* c_str() const { return (const char*)bytes(); }

private:
	// The String specification requires that all strings are stored, if possible, inline.
	// This means that if m_bytes_count is less-or-equal than the maximum number of bytes
	//   that can be stored inline, the data is stored in the sso buffer.
	ALWAYS_INLINE NODISCARD bool is_using_sso() const
	{
		return m_bytes_count <= SSOSize;
	}

private:
	// Expects m_bytes_count to be 0, because allocate() might be called.
	ALWAYS_INLINE void construct_from_span(Span<const char> chars_span)
	{
		// The String specification requires to always store the string in the inlined buffer,
		//   when it is possible.
		// So, we check to see if it is possible.
		if (chars_span.bytes_count() < SSOSize)
		{
			// The provided span can fit on the sso buffer.
			// So, we will copy the string into the inlined buffer.

			m_bytes_count = chars_span.bytes_count() + 1;
			Memory::copy(m_sso_bytes, chars_span.bytes(), chars_span.bytes_count());
			m_sso_bytes[m_bytes_count - 1] = 0;
		}
		else
		{
			// The allocate function also sets the m_bytes_count, so we don't have to explicitly do it.
			allocate(chars_span.bytes_count() + 1);

			Memory::copy(m_heap_bytes, chars_span.bytes(), chars_span.bytes_count());
			m_heap_bytes[m_bytes_count - 1] = 0;
		}
	}

	ALWAYS_INLINE void assign_from_span(Span<const char> chars_span)
	{
		// The String specification requires to always store the string in the inlined buffer,
		//   when it is possible.
		// So, we check to see if it is possible.
		if (chars_span.bytes_count() < SSOSize)
		{
			// The provided span can fit on the sso buffer.
			// So, we will copy the string into the inlined buffer.

			// If data is currently stored on the heap, we release the memory.
			release_memory_check();

			m_bytes_count = chars_span.bytes_count() + 1;
			Memory::copy(m_sso_bytes, chars_span.bytes(), chars_span.bytes_count());
			m_sso_bytes[m_bytes_count - 1] = 0;
		}
		else
		{
			// Only if m_bytes_count is exactly the same as the chars_span bytes_count we don't have to
			//   allocate a new block.
			// The String specification requires the heap buffer to be exactly m_bytes_count.
			// So, even if we already have enough space for the span, we have to allocate a new memory block.
			if (m_bytes_count != (chars_span.bytes_count() + 1))
			{
				// The allocate function also sets the m_bytes_count, so we don't have to explicitly do it.
				allocate(chars_span.bytes_count() + 1);
			}

			Memory::copy(m_heap_bytes, chars_span.bytes(), chars_span.bytes_count());
			m_heap_bytes[m_bytes_count - 1] = 0;
		}
	}

	// Allocates a memory block on the heap for the string.
	// It expects new_bytes_count to be greater than the maximum number of bytes that can be stored inlined.
	//   Also, the new_bytes_count must be different than m_bytes_count, so we don't have unnecessary reallocations.
	void allocate(usize new_bytes_count)
	{
		HC_ASSERT(new_bytes_count != m_bytes_count && new_bytes_count > SSOSize);

		release_memory_check();
		m_heap_bytes = (char*)Memory::allocate_tagged_i(new_bytes_count);
		m_bytes_count = new_bytes_count;
	}

	// Releases the heap buffer from memory.
	// It is expected that the string uses, at the moment of calling this, the heap buffer.
	// If the string doesn't store the data on the heap, this function will cause a crash.
	void release_memory()
	{
		Memory::free(m_heap_bytes);
	}

	// If the data is stored on the heap, this function releases the heap buffer from memory.
	// This function first checks if the data is stored on the heap.
	// By doing it, this function is safe to call no matter where the buffer is being stored.
	//   If the buffer is inlined, calling this will do nothing.
	ALWAYS_INLINE void release_memory_check()
	{
		if (!is_using_sso())
		{
			release_memory();
		}
	}

private:
	union
	{
		// Pointer to the buffer stored on the heap. Has a valid value only if the string doesn't
		//   use SSO to store its data.
		char* m_heap_bytes;

		// The inlined SSO buffer. Contains valid data only if the string uses SSO to store its data.
		char m_sso_bytes[SSOSize];
	};

	// The number of bytes the string occupies. This values includes the null-terminated byte/character.
	usize m_bytes_count;
};

} // namespace HC