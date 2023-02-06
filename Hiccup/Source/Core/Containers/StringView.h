// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

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

    ALWAYS_INLINE constexpr StringView(const char* utf8_string, size_t utf8_string_bytes_count)
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
    ALWAYS_INLINE const char* c_str() const { return (const char*)bytes(); }

    // The read/write modifiers of the bytes are given by the const-ness of the function.
    ALWAYS_INLINE ReadonlyBytes bytes() const { return m_view.bytes(); }
    ALWAYS_INLINE ReadWriteBytes bytes() { return m_view.bytes(); }

    // Wrappers around 'bytes', explicitly specifying the bytes' read/write modifiers.
    ALWAYS_INLINE ReadonlyBytes readonly_bytes() const { return bytes(); }
    ALWAYS_INLINE WriteonlyBytes writeonly_bytes() { return bytes(); }
    ALWAYS_INLINE ReadWriteBytes read_write_bytes() { return bytes(); }

    ALWAYS_INLINE size_t bytes_count() const { return m_view.bytes_count(); }

    // The read/write modifiers of the bytes are given by the const-ness of the function.
    ALWAYS_INLINE Span<ReadonlyByte> bytes_span() const { return Span<ReadonlyByte>(bytes(), bytes_count()); }
    ALWAYS_INLINE Span<ReadWriteByte> bytes_span() { return Span<ReadWriteByte>(bytes(), bytes_count()); }

    // Wrappers around 'bytes_span', explicitly specifying the bytes' read/write modifiers.
    ALWAYS_INLINE Span<ReadonlyByte> readonly_bytes_span() const { return bytes_span(); }
    ALWAYS_INLINE Span<WriteonlyByte> writeonly_byte_spans() { return bytes_span(); }
    ALWAYS_INLINE Span<ReadWriteByte> read_write_byt_spanes() { return bytes_span(); }

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