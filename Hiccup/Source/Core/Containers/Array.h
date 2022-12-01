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
	ALWAYS_INLINE T* Data() const;

	/** @return The number of elements that are currently stored in the container. */
	ALWAYS_INLINE usize Size() const;

	/** @return The capacity (in elements) of the array's memory block. */
	ALWAYS_INLINE usize Capacity() const;

	/** @return True if the container has no elements stored; False otherwise. */
	ALWAYS_INLINE bool IsEmpty() const;

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
	ALWAYS_INLINE T& At(usize index);

	/**
	 * Gets the element stored at the specified index.
	 * Const version of the above.
	 *
	 * @param index The index of the element. It must be less than the array's size.
	 *
	 * @return Reference to the element stored at the specified index.
	 */
	ALWAYS_INLINE const T& At(usize index) const;

	/** @return The first element in the array. */
	ALWAYS_INLINE T& Front();

	/** @return The first element in the array. */
	ALWAYS_INLINE const T& Front() const;

	/** @return The last element in the array. */
	ALWAYS_INLINE T& Back();

	/** @return The last element in the array. */
	ALWAYS_INLINE const T& Back() const;

public:
	/**
	 * Adds an element to the container.
	 * Because the memory is contiguous, the element is stored at the end of the array.
	 * 
	 * @param element The element to add.
	 * 
	 * @return Reference to the new element (that is stored in the array).
	 */
	T& Add(const T& element);

	/**
	 * Adds an element to the container.
	 * Because the memory is contiguous, the element is stored at the end of the array.
	 * Move semantics version of the above. The element is moved to the array (instead of copied).
	 * 
	 * @param element The element to add.
	 * 
	 * @return Reference to the new element (that is stored in the array).
	 */
	T& Add(T&& element);

	/**
	 * Adds an element to the container.
	 * The element is constructed directly in place, passing the arguments to the object's constructor.
	 * 
	 * @param args... The parameters used by the object's constructor.
	 * 
	 * @return Reference to the newly created element.
	 */
	template<typename... Args>
	T& EmplaceBack(Args&&... args);

	/**
	 * Adds an element to the container.
	 * The element is created using its default constructor
	 * 
	 * @return Reference to the newly created element.
	 */
	T& AddDefaulted();

	/**
	 * Adds elements to the container.
	 * The elements are initialized using their default constructor.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize AddDefaulted(usize count);

	/**
	 * Adds an element to the container.
	 * The element is initialized by setting its memory to zero.
	 * 
	 * @return The index of the added element.
	 */
	usize AddZeroed();

	/**
	 * Adds elements to the container.
	 * The elements are initialized by setting their memory to zero.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize AddZeroed(usize count);

	/**
	 * Adds an element to the container.
	 * The element is not initialized in any way.
	 * 
	 * @return The index of the added element.
	 */
	usize AddUninitialized();

	/**
	 * Adds elements to the container.
	 * The elements are not initialized in any way.
	 * 
	 * @param count The number of elements to add.
	 * 
	 * @return The index of the first added element.
	 */
	usize AddUninitialized(usize count);

public:
	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are initialized
	 *   with their default constructor.
	 * 
	 * @param newSize The array's new size.
	 */
	void SetSizeDefaulted(usize newSize);

	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are initialized
	 *   by setting their memory to zero.
	 * 
	 * @param newSize The array's new size.
	 */
	void SetSizeZeroed(usize newSize);

	/**
	 * Resizes the array.
	 * If the new size is greater than the old size, the new elements are not
	 *   initialized in any way.
	 * 
	 * @param newSize The array's new size.
	 */
	void SetSizeUninitialized(usize newSize);

	/**
	 * Resizes the array by just setting its size to the new value.
	 * This function can easily be a source of bugs.
	 * 
	 * @param newSize The array's new size.
	 */
	void SetSizeInternal(usize newSize);

	/**
	 * Sets the array's capacity to the specified value, by reallocating the array's memory buffer.
	 * 
	 * @param newCapacity The array's memory buffer's new capacity.
	 */
	void SetCapacity(usize newCapacity);

	/**
	 * Clears the array. It doesn't shrink the memory block.
	 */
	void Clear();

private:
	ALWAYS_INLINE bool ShouldGrow(usize requiredAdditionalSize);

	/** @return The array's natural growth. */
	ALWAYS_INLINE usize NextCapacity() const;

	/** @return Whether the array's natural growth, or the minimum required size. */
	ALWAYS_INLINE usize NextCapacity(usize requiredAdditionalSize) const;

	/**
	 * ReAllocates the memory block. The new block can be both bigger and smaller.
	 * All the elements are moved into the new memory block.
	 * 
	 * @param newCapacity The capacity of the new memory block.
	 */
	void ReAllocate(usize newCapacity);

	/**
	 * ReAllocates the memory block. The new block can be both bigger and smaller.
	 * The elements are not transfered to the new memory block.
	 *
	 * @param newCapacity The capacity of the new memory block.
	 */
	void ReAllocateNoCopy(usize newCapacity);

	/**
	 * Deletes the memory block.
	 */
	void DeleteData();

private:
	// Pointer to the memory block that holds the array's elements.
	T* m_Data;

	// The capacity (in elements) of the memory block.
	usize m_Capacity;

	// The number of valid elements currently stored in the array.
	usize m_Size;

	// The allocator instance used to perform all memory allocations/deallocations.
	AllocatorType m_AllocatorInstance;
};

//////////////// ARRAY IMPLEMENTATION ////////////////

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array()
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_Size(0)
{}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array(const Array<T, AllocatorType>& other)
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_Size(0)
{
	ReAllocateNoCopy(other.m_Size);
	m_Size = other.m_Size;

	for (usize i = 0; i < m_Size; ++i)
	{
		new (m_Data + i) T(other.m_Data[i]);
	}
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::Array(Array<T, AllocatorType>&& other) noexcept
	: m_Data(other.m_Data)
	, m_Capacity(other.m_Capacity)
	, m_Size(other.m_Size)
{
	other.m_Data = nullptr;
	other.m_Capacity = 0;
	other.m_Size = 0;
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>::~Array()
{
	Clear();
	DeleteData();
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>& Array<T, AllocatorType>::operator=(const Array<T, AllocatorType>& other)
{
	if (other.m_Size > m_Capacity)
	{
		ReAllocateNoCopy(other.m_Size);
	}
	else
	{
		Clear();
	}

	m_Size = other.m_Size;
	for (usize i = 0; i < m_Size; ++i)
	{
		new (m_Data + i) T(other.m_Data[i]);
	}

	return *this;
}

template<typename T, typename AllocatorType>
Array<T, AllocatorType>& Array<T, AllocatorType>::operator=(Array<T, AllocatorType>&& other) noexcept
{
	Clear();

	T* tempData = m_Data;
	usize tempCapacity = m_Capacity;

	m_Data = other.m_Data;
	m_Capacity = other.m_Capacity;
	m_Size = other.m_Size;

	other.m_Data = tempData;
	other.m_Capacity = tempCapacity;
	other.m_Size = 0;

	return *this;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T* Array<T, AllocatorType>::Data() const
{
	return m_Data;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::Size() const
{
	return m_Size;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::Capacity() const
{
	return m_Capacity;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE bool Array<T, AllocatorType>::IsEmpty() const
{
	return (m_Size == 0);
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::operator[](usize index)
{
	HC_ASSERT(index < m_Size); // Index out of range!
	return m_Data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::operator[](usize index) const
{
	HC_ASSERT(index < m_Size); // Index out of range!
	return m_Data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::At(usize index)
{
	HC_ASSERT(index < m_Size); // Index out of range!
	return m_Data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::At(usize index) const
{
	HC_ASSERT(index < m_Size); // Index out of range!
	return m_Data[index];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::Front()
{
	HC_ASSERT(m_Size > 0); // The array is empty!
	return m_Data[0];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::Front() const
{
	HC_ASSERT(m_Size > 0); // The array is empty!
	return m_Data[0];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE T& Array<T, AllocatorType>::Back()
{
	HC_ASSERT(m_Size > 0); // The array is empty!
	return m_Data[m_Size - 1];
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE const T& Array<T, AllocatorType>::Back() const
{
	HC_ASSERT(m_Size > 0); // The array is empty!
	return m_Data[m_Size - 1];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::Add(const T& element)
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	new (m_Data + m_Size) T(element);
	return m_Data[m_Size++];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::Add(T&& element)
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	new (m_Data + m_Size) T(Types::Move(element));
	return m_Data[m_Size++];
}

template<typename T, typename AllocatorType>
template<typename... Args>
T& Array<T, AllocatorType>::EmplaceBack(Args&&... args)
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	new (m_Data + m_Size) T(Types::Forward<Args>(args)...);
	return m_Data[m_Size++];
}

template<typename T, typename AllocatorType>
T& Array<T, AllocatorType>::AddDefaulted()
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	new (m_Data + m_Size) T();
	return m_Data[m_Size++];
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::AddDefaulted(usize count)
{
	if (ShouldGrow(count))
	{
		ReAllocate(NextCapacity(count));
	}

	const usize oldSize = m_Size;
	m_Size += count;

	for (usize i = oldSize; i < m_Size; ++i)
	{
		new (m_Data + i) T();
	}

	return oldSize;
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::AddZeroed()
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	Memory::Zero(m_Data + m_Size, sizeof(T));
	return (m_Size++);
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::AddZeroed(usize count)
{
	if (ShouldGrow(count))
	{
		ReAllocate(NextCapacity(count));
	}

	Memory::Zero(m_Data + m_Size, count * sizeof(T));
	const usize oldSize = m_Size;
	m_Size += count;
	return oldSize;
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::AddUninitialized()
{
	if (ShouldGrow(1))
	{
		ReAllocate(NextCapacity());
	}

	return (m_Size++);
}

template<typename T, typename AllocatorType>
usize Array<T, AllocatorType>::AddUninitialized(usize count)
{
	if (ShouldGrow(count))
	{
		ReAllocate(NextCapacity(count));
	}

	const usize oldSize = m_Size;
	m_Size += count;
	return oldSize;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::SetSizeDefaulted(usize newSize)
{
	usize oldSize = m_Size;
	SetSizeUninitialized(newSize);

	for (usize index = oldSize; index < m_Size; ++index)
	{
		new (m_Data + index) T();
	}
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::SetSizeZeroed(usize newSize)
{
	usize oldSize = m_Size;
	SetSizeUninitialized(newSize);

	if (m_Size > oldSize)
	{
		Memory::Zero(m_Data + oldSize, (m_Size - oldSize) * sizeof(T));
	}
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::SetSizeUninitialized(usize newSize)
{
	if (newSize > m_Capacity)
	{
		ReAllocate(NextCapacity(newSize - m_Size));
	}
	else
	{
		for (usize index = newSize; index < m_Size; ++index)
		{
			m_Data[index].~T();
		}
	}

	m_Size = newSize;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::SetSizeInternal(usize newSize)
{
	m_Size = newSize;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::SetCapacity(usize newCapacity)
{
	if (newCapacity == m_Capacity)
	{
		return;
	}

	ReAllocate(newCapacity);
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::Clear()
{
	for (usize i = 0; i < m_Size; ++i)
	{
		m_Data[i].~T();
	}
	m_Size = 0;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE bool Array<T, AllocatorType>::ShouldGrow(usize requiredAdditionalSize)
{
	return m_Size + requiredAdditionalSize > m_Capacity;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::NextCapacity() const
{
	return m_Capacity + m_Capacity / 2 + 1;
}

template<typename T, typename AllocatorType>
ALWAYS_INLINE usize Array<T, AllocatorType>::NextCapacity(usize requiredAdditionalSize) const
{
	const usize nextCapacity = NextCapacity();
	const usize requiredCapacity = m_Size + requiredAdditionalSize;
	return nextCapacity > requiredCapacity ? nextCapacity : requiredCapacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::ReAllocate(usize newCapacity)
{
	T* newBlock = (T*)m_AllocatorInstance.AllocateTaggedI(newCapacity * sizeof(T));

	if (m_Size > newCapacity)
	{
		for (usize i = newCapacity; i < m_Size; ++i)
		{
			m_Data[i].~T();
		}
		m_Size = newCapacity;
	}

	for (usize i = 0; i < m_Size; ++i)
	{
		new (newBlock + i) T(Types::Move(m_Data[i]));
		m_Data[i].~T();
	}

	DeleteData();

	m_Data = newBlock;
	m_Capacity = newCapacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::ReAllocateNoCopy(usize newCapacity)
{
	T* newBlock = (T*)m_AllocatorInstance.AllocateTaggedI(newCapacity * sizeof(T));

	Clear();
	DeleteData();

	m_Data = newBlock;
	m_Capacity = newCapacity;
}

template<typename T, typename AllocatorType>
void Array<T, AllocatorType>::DeleteData()
{
	m_AllocatorInstance.Free(m_Data, m_Capacity * sizeof(T));
}

} // namespace HC