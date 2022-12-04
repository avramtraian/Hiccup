// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup Container - Array.
 *----------------------------------------------------------------
 * Container that holds all the elements contiguous in memory. It is an alternative
 *   to 'std::vector'. This should be used because it provides better debuggability
 *   and maintainability, as well as having consistency throughout the codebase.
 */
template<typename T, typename AllocatorType = HeapAllocator>
class Array
{
public:
	/**
	 * Default constructor.
	 */
	Array();

	/**
	 * Copy constructor.
	 * 
	 * @param other The array to copy.
	 */
	Array(const Array<T, AllocatorType>& other);

	/**
	 * Move constructor.
	 * 
	 * @param other The array to move.
	 */
	Array(Array<T, AllocatorType>&& other) noexcept;

	/**
	 * Destructor.
	 */
	~Array();

	/**
	 * Copy assignment operator.
	 * 
	 * @param other The array to copy.
	 * 
	 * @return Reference to this, after the copy.
	 */
	Array<T, AllocatorType>& operator=(const Array<T, AllocatorType>& other);

	/**
	 * Move assignment operator.
	 * 
	 * @param other The array to move.
	 * 
	 * @return Reference to this, after the move.
	 */
	Array<T, AllocatorType>& operator=(Array<T, AllocatorType>&& other) noexcept;

public:
	/** @return Pointer to the array's memory block. */
	ALWAYS_INLINE T* data() const;

	/** @return The number of elements that are currently stored in the container. */
	ALWAYS_INLINE usize size() const;

	/** @return The capacity (in elements) of the array's memory block. */
	ALWAYS_INLINE usize capacity() const;

	/** @return True if the container has no elements stored; False otherwise. */
	ALWAYS_INLINE bool is_empty() const;

public:
	/**
	 * Index operator. Gets the element stored at the specified index.
	 * 
	 * @param index The index of the element. It must be less than the array's size.
	 * 
	 * @return Reference to the element stored at the specified index.
	 */
	ALWAYS_INLINE T& operator[](usize index);

	/**
	 * Index operator. Gets the element stored at the specified index.
	 * Const version of the above.
	 * 
	 * @param index The index of the element. It must be less than the array's size.
	 *
	 * @return Reference to the element stored at the specified index.
	 */
	ALWAYS_INLINE const T& operator[](usize index) const;

	/**
	 * Gets the element stored at the specified index.
	 * 
	 * @param index The index of the element. It must be less than the array's size.
	 * 
	 * @return Reference to the element stored at the specified index.
	 */
	ALWAYS_INLINE T& at(usize index);

	/**
	 * Gets the element stored at the specified index.
	 * Const version of the above.
	 *
	 * @param index The index of the element. It must be less than the array's size.
	 *
	 * @return Reference to the element stored at the specified index.
	 */
	ALWAYS_INLINE const T& at(usize index) const;

	/** @return The first element in the array. */
	ALWAYS_INLINE T& front();

	/** @return The first element in the array. */
	ALWAYS_INLINE const T& front() const;

	/** @return The last element in the array. */
	ALWAYS_INLINE T& back();

	/** @return The last element in the array. */
	ALWAYS_INLINE const T& back() const;

public:
	/**
	 * Adds an element to the container.
	 * Because the memory is contiguous, the element is stored at the end of the array.
	 * 
	 * @param element The element to add.
	 * 
	 * @return Reference to the new element (that is stored in the array).
	 */
	T& add(const T& element);

	/**
	 * Adds an element to the container.
	 * Because the memory is contiguous, the element is stored at the end of the array.
	 * Move semantics version of the above. The element is moved to the array (instead of copied).
	 * 
	 * @param element The element to add.
	 * 
	 * @return Reference to the new element (that is stored in the array).
	 */
	T& add(T&& element);

	/**
	 * Adds an element to the container.
	 * The element is constructed directly in place, passing the arguments to the object's constructor.
	 * 
	 * @param args... The parameters used by the object's constructor.
	 * 
	 * @return Reference to the newly created element.
	 */
	template<typename... Args>
	T& emplace_back(Args&&... args);

	/**
	 * Adds an element to the container.
	 * The element is created using its default constructor
	 * 
	 * @return Reference to the newly created element.
	 */
	T& add_defaulted();

	/**
	 * Adds elements to the container.
	 * The elements are initialized using their default constructor.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize add_defaulted(usize count);

	/**
	 * Adds an element to the container.
	 * The element is initialized by setting its memory to zero.
	 * 
	 * @return The index of the added element.
	 */
	usize add_zeroed();

	/**
	 * Adds elements to the container.
	 * The elements are initialized by setting their memory to zero.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize add_zeroed(usize count);

	/**
	 * Adds an element to the container.
	 * The element is not initialized in any way.
	 * 
	 * @return The index of the added element.
	 */
	usize add_uninitialized();

	/**
	 * Adds elements to the container.
	 * The elements are not initialized in any way.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize add_uninitialized(usize count);

public:
	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are initialized
	 *   with their default constructor.
	 * 
	 * @param new_size The array's new size.
	 */
	void set_size_defaulted(usize new_size);

	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are initialized
	 *   by setting their memory to zero.
	 * 
	 * @param new_size The array's new size.
	 */
	void set_size_zeroed(usize new_size);

	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are not
	 *   initialized in any way.
	 * 
	 * @param new_size The array's new size.
	 */
	void set_size_uninitialized(usize new_size);

	/**
	 * Resizes the array by just setting its size to the new value.
	 * This function can easily be a source of bugs.
	 * 
	 * @param new_size The array's new size.
	 */
	void set_size_internal(usize new_size);

	/**
	 * Sets the array's capacity to the specified value, by reallocating the array's memory buffer.
	 * 
	 * @param new_capacity The array's memory buffer's new capacity.
	 */
	void set_capacity(usize new_capacity);

	/**
	 * Clears the array. It doesn't shrink the memory block.
	 */
	void clear();

private:
	// Checks whether or not the container can store an additional number of elements, without reallocating.
	ALWAYS_INLINE bool should_grow(usize required_additional_size) const;

	/** @return The array's natural growth. */
	ALWAYS_INLINE usize calculate_growth() const;

	/** @return Whether the array's natural growth, or the minimum required size. */
	ALWAYS_INLINE usize calculate_growth(usize required_additional_size) const;

	/**
	 * ReAllocates the memory block. The new block can be both bigger and smaller.
	 * All the elements are moved into the new memory block.
	 * 
	 * @param new_capacity The capacity of the new memory block.
	 */
	void re_allocate(usize new_capacity);

	/**
	 * ReAllocates the memory block. The new block can be both bigger and smaller.
	 * The elements are not transfered to the new memory block.
	 *
	 * @param new_capacity The capacity of the new memory block.
	 */
	void re_allocate_no_copy(usize new_capacity);

	/**
	 * Deletes the memory block.
	 */
	void release_memory();

private:
	// Pointer to the memory block that holds the array's elements.
	T* m_data;

	// The capacity (in elements) of the memory block.
	usize m_capacity;

	// The number of valid elements currently stored in the array.
	usize m_size;

	// The allocator instance used to perform all memory allocations/deallocations.
	AllocatorType m_allocator_instance;
};

//////////////// ARRAY IMPLEMENTATION ////////////////

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array()
	: m_data(nullptr)
	, m_capacity(0)
	, m_size(0)
{}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array(const Array<T, AllocatorType>& other)
	: m_data(nullptr)
	, m_capacity(0)
	, m_size(0)
{
	re_allocate_no_copy(other.m_size);
	m_size = other.m_size;

	for (usize i = 0; i < m_size; ++i)
	{
		new (m_data + i) T(other.m_data[i]);
	}
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array(Array<T, AllocatorType>&& other) noexcept
	: m_data(other.m_data)
	, m_capacity(other.m_capacity)
	, m_size(other.m_size)
{
	other.m_data = nullptr;
	other.m_capacity = 0;
	other.m_size = 0;
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::~Array()
{
	clear();
	release_memory();
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>& Array<T, AllocatorType>::operator=(const Array<T, AllocatorType>& other)
{
	if (other.m_size > m_capacity)
	{
		re_allocate_no_copy(other.m_size);
	}
	else
	{
		clear();
	}

	m_size = other.m_size;
	for (usize i = 0; i < m_size; ++i)
	{
		new (m_data + i) T(other.m_data[i]);
	}

	return *this;
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>& Array<T, AllocatorType>::operator=(Array<T, AllocatorType>&& other) noexcept
{
	clear();

	T* temp_data = m_data;
	usize temp_capacity = m_capacity;

	m_data = other.m_data;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_data = temp_data;
	other.m_capacity = temp_capacity;
	other.m_size = 0;

	return *this;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T* Array<T, AllocatorType>::data() const
{
	return m_data;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::size() const
{
	return m_size;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::capacity() const
{
	return m_capacity;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE bool Array<T, AllocatorType>::is_empty() const
{
	return (m_size == 0);
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::operator[](usize index)
{
	HC_ASSERT(index < m_size); // Index out of range!
	return m_data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::operator[](usize index) const
{
	HC_ASSERT(index < m_size); // Index out of range!
	return m_data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::at(usize index)
{
	HC_ASSERT(index < m_size); // Index out of range!
	return m_data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::at(usize index) const
{
	HC_ASSERT(index < m_size); // Index out of range!
	return m_data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::front()
{
	HC_ASSERT(m_size > 0); // The array is empty!
	return m_data[0];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::front() const
{
	HC_ASSERT(m_size > 0); // The array is empty!
	return m_data[0];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::back()
{
	HC_ASSERT(m_size > 0); // The array is empty!
	return m_data[m_size - 1];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::back() const
{
	HC_ASSERT(m_size > 0); // The array is empty!
	return m_data[m_size - 1];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::add(const T& element)
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	new (m_data + m_size) T(element);
	return m_data[m_size++];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::add(T&& element)
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	new (m_data + m_size) T(Types::Move(element));
	return m_data[m_size++];
}

template<typename T, typename AllocatorType>
template<typename... Args>
T& Array<T, AllocatorType>::emplace_back(Args&&... args)
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	new (m_data + m_size) T(Types::Forward<Args>(args)...);
	return m_data[m_size++];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::add_defaulted()
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	new (m_data + m_size) T();
	return m_data[m_size++];
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::add_defaulted(usize count)
{
	if (should_grow(count))
	{
		re_allocate(calculate_growth(count));
	}

	const usize old_size = m_size;
	m_size += count;

	for (usize i = old_size; i < m_size; ++i)
	{
		new (m_data + i) T();
	}

	return old_size;
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::add_zeroed()
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	Memory::zero(m_data + m_size, sizeof(T));
	return (m_size++);
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::add_zeroed(usize count)
{
	if (should_grow(count))
	{
		re_allocate(calculate_growth(count));
	}

	Memory::zero(m_data + m_size, count * sizeof(T));
	const usize old_size = m_size;
	m_size += count;
	return old_size;
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::add_uninitialized()
{
	if (should_grow(1))
	{
		re_allocate(calculate_growth());
	}

	return (m_size++);
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::add_uninitialized(usize count)
{
	if (should_grow(count))
	{
		re_allocate(calculate_growth(count));
	}

	const usize old_size = m_size;
	m_size += count;
	return old_size;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::set_size_defaulted(usize new_size)
{
	const usize old_size = m_size;
	set_size_uninitialized(new_size);

	for (usize index = old_size; index < m_size; ++index)
	{
		new (m_data + index) T();
	}
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::set_size_zeroed(usize new_size)
{
	const usize old_size = m_size;
	set_size_uninitialized(new_size);

	if (m_size > old_size)
	{
		Memory::zero(m_data + old_size, (m_size - old_size) * sizeof(T));
	}
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::set_size_uninitialized(usize new_size)
{
	if (new_size > m_capacity)
	{
		re_allocate(calculate_growth(new_size - m_size));
	}
	else
	{
		for (usize index = new_size; index < m_size; ++index)
		{
			m_data[index].~T();
		}
	}

	m_size = new_size;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::set_size_internal(usize new_size)
{
	m_size = new_size;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::set_capacity(usize new_capacity)
{
	if (new_capacity == m_capacity)
	{
		return;
	}

	re_allocate(new_capacity);
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::clear()
{
	for (usize i = 0; i < m_size; ++i)
	{
		m_data[i].~T();
	}
	m_size = 0;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE bool Array<T, AllocatorType>::should_grow(usize required_additional_size) const
{
	return m_size + required_additional_size > m_capacity;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::calculate_growth() const
{
	return m_capacity + m_capacity / 2 + 1;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::calculate_growth(usize required_additional_size) const
{
	const usize natural_growth = calculate_growth();
	const usize requiredCapacity = m_size + required_additional_size;
	return natural_growth > requiredCapacity ? natural_growth : requiredCapacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::re_allocate(usize new_capacity)
{
	T* new_data = (T*)m_allocator_instance.allocate_tagged_i(new_capacity * sizeof(T));

	if (m_size > new_capacity)
	{
		for (usize i = new_capacity; i < m_size; ++i)
		{
			m_data[i].~T();
		}
		m_size = new_capacity;
	}

	for (usize i = 0; i < m_size; ++i)
	{
		new (new_data + i) T(Types::Move(m_data[i]));
		m_data[i].~T();
	}

	release_memory();

	m_data = new_data;
	m_capacity = new_capacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::re_allocate_no_copy(usize new_capacity)
{
	T* new_data = (T*)m_allocator_instance.allocate_tagged_i(new_capacity * sizeof(T));

	clear();
	release_memory();

	m_data = new_data;
	m_capacity = new_capacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::release_memory()
{
	m_allocator_instance.free(m_data, m_capacity * sizeof(T));
}

} // namespace HC