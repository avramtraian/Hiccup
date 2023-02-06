// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

#include "Span.h"

namespace HC
{

//----------------------------------------------------------------
// Hiccup Container - Array.
//----------------------------------------------------------------
// Container that holds all the elements contiguous in memory. It is an alternative
//   to 'std::vector'. This should be used because it provides better debuggability
//   and maintainability, as well as having consistency throughout the codebase.
template<typename T, typename AllocatorType = HeapAllocator>
class Array
{
public:
    Array()
        : m_data(nullptr)
        , m_capacity(0)
        , m_size(0)
    {}

    Array(const Array<T, AllocatorType>& other)
        : m_data(nullptr)
        , m_capacity(0)
        , m_size(0)
    {
        re_allocate_no_copy(other.m_size);
        m_size = other.m_size;

        for (size_t i = 0; i < m_size; ++i) {
            new (m_data + i) T(other.m_data[i]);
        }
    }

    Array(Array<T, AllocatorType>&& other) noexcept
        : m_data(other.m_data)
        , m_capacity(other.m_capacity)
        , m_size(other.m_size)
    {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    ~Array()
    {
        clear();
        release_memory();
    }

    Array<T, AllocatorType>& operator=(const Array<T, AllocatorType>& other)
    {
        if (other.m_size > m_capacity) {
            re_allocate_no_copy(other.m_size);
        }
        else {
            clear();
        }

        m_size = other.m_size;
        for (size_t i = 0; i < m_size; ++i) {
            new (m_data + i) T(other.m_data[i]);
        }

        return *this;
    }

    Array<T, AllocatorType>& operator=(Array<T, AllocatorType>&& other) noexcept
    {
        clear();

        T* temp_data = m_data;
        size_t temp_capacity = m_capacity;

        m_data = other.m_data;
        m_capacity = other.m_capacity;
        m_size = other.m_size;

        other.m_data = temp_data;
        other.m_capacity = temp_capacity;
        other.m_size = 0;

        return *this;
    }

public:
    ALWAYS_INLINE T* data() const { return m_data }

    ALWAYS_INLINE size_t size() const { return m_size; }

    ALWAYS_INLINE size_t capacity() const { return m_capacity; }

    ALWAYS_INLINE bool is_empty() const { return (m_size == 0); }

    ALWAYS_INLINE NODISCARD Span<T> span() const { return Span<T>(m_data, m_size); }

public:
    ALWAYS_INLINE T& operator[](size_t index)
    {
        HC_ASSERT(index < m_size); // Index out of range!
        return m_data[index];
    }

    ALWAYS_INLINE const T& operator[](size_t index) const
    {
        HC_ASSERT(index < m_size); // Index out of range!
        return m_data[index];
    }

    ALWAYS_INLINE T& at(size_t index)
    {
        HC_ASSERT(index < m_size); // Index out of range!
        return m_data[index];
    }

    ALWAYS_INLINE const T& at(size_t index) const
    {
        HC_ASSERT(index < m_size); // Index out of range!
        return m_data[index];
    }

    ALWAYS_INLINE T& front() { return at(0); }
    ALWAYS_INLINE const T& front() const { return at(0); }

    ALWAYS_INLINE T& back()
    {
        HC_ASSERT(m_size > 0); // The array is empty!
        return m_data[m_size - 1];
    }

    ALWAYS_INLINE const T& back() const
    {
        HC_ASSERT(m_size > 0); // The array is empty!
        return m_data[m_size - 1];
    }

public:
    T& add(const T& element)
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        new (m_data + m_size) T(element);
        return m_data[m_size++];
    }

    T& add(T&& element)
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        new (m_data + m_size) T(Types::move(element));
        return m_data[m_size++];
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        new (m_data + m_size) T(Types::forward<Args>(args)...);
        return m_data[m_size++];
    }

    T& add_defaulted()
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        new (m_data + m_size) T();
        return m_data[m_size++];
    }

    size_t add_defaulted(size_t count)
    {
        if (should_grow(count)) {
            re_allocate(calculate_growth(count));
        }

        const size_t old_size = m_size;
        m_size += count;

        for (size_t i = old_size; i < m_size; ++i) {
            new (m_data + i) T();
        }

        return old_size;
    }

    size_t add_zeroed()
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        Memory::zero(m_data + m_size, sizeof(T));
        return (m_size++);
    }

    size_t add_zeroed(size_t count)
    {
        if (should_grow(count)) {
            re_allocate(calculate_growth(count));
        }

        Memory::zero(m_data + m_size, count * sizeof(T));
        const size_t old_size = m_size;
        m_size += count;
        return old_size;
    }

    size_t add_uninitialized()
    {
        if (should_grow(1)) {
            re_allocate(calculate_growth());
        }

        return (m_size++);
    }

    size_t add_uninitialized(size_t count)
    {
        if (should_grow(count)) {
            re_allocate(calculate_growth(count));
        }

        const size_t old_size = m_size;
        m_size += count;
        return old_size;
    }

public:
    void pop()
    {
        HC_ASSERT(m_size > 0); // Trying to pop from an empty array!

        m_data[--m_size].~T();
    }

    void pop(size_t count)
    {
        HC_ASSERT(m_size >= count); // Trying to pop too many elements!

        for (size_t i = 0; i < count; ++i) {
            m_data[m_size - count - 1].~T();
        }
        m_size -= count;
    }

public:
    // Resizes the array. If the given size is greater than the current size, all newly included elements
    //   are initialized with their default constructor.
    // This function will cause a reallocation if the array's internal buffer is not big enough.
    void set_size_defaulted(size_t new_size)
    {
        // Save the current number of elements in the array.
        const size_t old_size = m_size;

        // Perform the actual resizing.
        set_size_uninitialized(new_size);

        // Construct the newly included elements in the array.
        for (size_t index = old_size; index < m_size; ++index) {
            new (m_data + index) T();
        }
    }

    void set_size_zeroed(size_t new_size)
    {
        const size_t old_size = m_size;
        set_size_uninitialized(new_size);

        if (m_size > old_size) {
            Memory::zero(m_data + old_size, (m_size - old_size) * sizeof(T));
        }
    }

    void set_size_uninitialized(size_t new_size)
    {
        if (new_size > m_capacity) {
            re_allocate(calculate_growth(new_size - m_size));
        }
        else {
            for (size_t index = new_size; index < m_size; ++index) {
                m_data[index].~T();
            }
        }

        m_size = new_size;
    }

    void set_size_internal(size_t new_size)
    {
        m_size = new_size;
    }

    void set_capacity(size_t new_capacity)
    {
        if (new_capacity == m_capacity) {
            return;
        }

        re_allocate(new_capacity);
    }

    void clear()
    {
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i].~T();
        }
        m_size = 0;
    }

private:
    // Checks whether or not the container can store an additional number of elements, without reallocating.
    ALWAYS_INLINE bool should_grow(size_t required_additional_size) const
    {
        return (m_size + required_additional_size) > m_capacity;
    }

    // Calculates the array's natural growth.
    ALWAYS_INLINE size_t calculate_growth() const
    {
        return m_capacity + m_capacity / 2 + 1;
    }

    // Whether the array's natural growth, or the minimum required size.
    ALWAYS_INLINE size_t calculate_growth(size_t required_additional_size) const
    {
        const size_t natural_growth = calculate_growth();
        const size_t requiredCapacity = m_size + required_additional_size;
        return natural_growth > requiredCapacity ? natural_growth : requiredCapacity;
    }

    void re_allocate(size_t new_capacity)
    {
        T* new_data = (T*)m_allocator_instance.allocate_tagged_i(new_capacity * sizeof(T));

        if (m_size > new_capacity) {
            for (size_t i = new_capacity; i < m_size; ++i)
            {
                m_data[i].~T();
            }
            m_size = new_capacity;
        }

        for (size_t i = 0; i < m_size; ++i) {
            new (new_data + i) T(Types::move(m_data[i]));
            m_data[i].~T();
        }

        release_memory();

        m_data = new_data;
        m_capacity = new_capacity;
    }

    void re_allocate_no_copy(size_t new_capacity)
    {
        T* new_data = (T*)m_allocator_instance.allocate_tagged_i(new_capacity * sizeof(T));

        clear();
        release_memory();

        m_data = new_data;
        m_capacity = new_capacity;
    }

    void release_memory()
    {
        m_allocator_instance.free(m_data, m_capacity * sizeof(T));
    }

private:
    // Pointer to the memory block that holds the array's elements.
    T* m_data;

    // The capacity (in elements) of the memory block.
    size_t m_capacity;

    // The number of valid elements currently stored in the array.
    size_t m_size;

    // The allocator instance used to perform all memory allocations/deallocations.
    AllocatorType m_allocator_instance;
};

} // namespace HC