// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Span.h"

namespace HC
{

// A linear, contiguous collection of elements.
// As the size is fixed at compile time, the elements are stored inline.
template<typename T, size_t C>
class FixedArray
{
public:
    // Iterator types.
    using Iterator = T*;
    using ConstIterator = const T*;
    using ReverseIterator = T*;
    using ReverseConstIterator = const T*;

public:
    // All copy/assignment constructors/operators are marked as default.
    // While this is unnecessary, it immediately tells the user that these
    // functions are meant to be used.
    FixedArray() = default;
    ~FixedArray() = default;

    FixedArray(const FixedArray&) = default;
    FixedArray(FixedArray&&) = default;

    FixedArray& operator=(const FixedArray&) = default;
    FixedArray& operator=(FixedArray&&) = default;

public:
    // Constexpr values of the array's size.
    ALWAYS_INLINE static constexpr size_t count() { return C; }
    ALWAYS_INLINE static constexpr size_t bytes_count() { return (C * sizeof(T)); }

    // Returns a pointer to the first elements of the array
    // The const-ness of the pointer is the same as the function's.
    ALWAYS_INLINE T* elements() { return &m_elements[0]; }
    ALWAYS_INLINE const T* elements() const { return &m_elements[0]; }

    // Returns a pointer to the first byte of the array.
    // The read/write modifiers of the bytes are given by the const-ness of the function.
    ALWAYS_INLINE ReadonlyBytes bytes() const { return (ReadonlyBytes)(elements()); }
    ALWAYS_INLINE ReadWriteBytes bytes() { return (ReadWriteBytes)(elements()); }

    // Wrappers around 'bytes', explicitly specifying the bytes' read/write modifiers.
    ALWAYS_INLINE ReadonlyBytes readonly_bytes() const { return bytes(); }
    ALWAYS_INLINE WriteonlyBytes writeonly_bytes() { return bytes(); }
    ALWAYS_INLINE ReadWriteBytes read_write_bytes() { return bytes(); }

public:
    ALWAYS_INLINE T& at(size_t index)
    {
        // Index is out of range.
        HC_ASSERT(index < count());
        return m_elements[index];
    }

    ALWAYS_INLINE const T& at(size_t index) const
    {
        // Index is out of range.
        HC_ASSERT(index < count());
        return m_elements[index];
    }

    ALWAYS_INLINE T& operator[](size_t index) { return at(index); }
    ALWAYS_INLINE const T& operator[](size_t index) const { return at(index); }

    ALWAYS_INLINE T& front() { return m_elements[0]; }
    ALWAYS_INLINE const T& front() const { return m_elements[0]; }

    ALWAYS_INLINE T& back() { return m_elements[count() - 1]; }
    ALWAYS_INLINE const T& back() const { return m_elements[count() - 1]; }

public:
    // Non-const forward iterators.
    Iterator begin() { return Iterator(elements()); }
    Iterator end() { return Iterator(elements() + count()); }

    // Const forward iterators.
    ConstIterator begin() const { return ConstIterator(elements()); }
    ConstIterator end() const { return ConstIterator(elements() + count()); }

    // Non-const reverse iterators.
    ReverseIterator rbegin() { return ReverseIterator(elements() + count() - 1); }
    ReverseIterator rend() { return ReverseIterator(elements() - 1); }

    // Const reverse iterators.
    ReverseConstIterator rbegin() const { return ReverseConstIterator(elements() + count() - 1); }
    ReverseConstIterator rend() const { return ReverseConstIterator(elements() - 1); }

private:
    // The elements stored in the array.
    T m_elements[C];

    static_assert(C > 0, "A 'FixedArray' container must have a size greater 0!");
};

} // namespace HC
