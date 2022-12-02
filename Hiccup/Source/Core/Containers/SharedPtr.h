// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

/**
 * This should be included in every class/struct that uses automatic
 *   reference counting.
 * 
 * Usage:
 *   class/struct SomeObject
 *   {
 *       HC_SHARED_OBJECT;
 * 
 *   public/private:
 *       // ...
 *   };
 */
#define HC_SHARED_OBJECT                \
	private:                            \
		uint64 __m_ReferenceCount = 0;  \
		                                \
		template<typename T>            \
		friend class SharedPtr;

namespace HC
{

/**
 *----------------------------------------------------------------
 * Hiccup SharedPtr.
 *----------------------------------------------------------------
 * Automatic intrusive reference counted object holder.
 * 
 * @tparam T A subclass of RefCounted.
 */
template<typename T>
class SharedPtr
{
public:
	/**
	 * Default constructor.
	 */
	SharedPtr();

	/**
	 * Copy constructor.
	 * 
	 * @param other The shared pointer to copy.
	 */
	SharedPtr(const SharedPtr<T>& other);

	/**
	 * Move constructor.
	 * 
	 * @param other The shared pointer to move.
	 */
	SharedPtr(SharedPtr<T>&& other) noexcept;

	/**
	 * Constructs a shared pointer from a raw pointer.
	 * 
	 * @param pointer The raw pointer to create the shared pointer from.
	 */
	explicit SharedPtr(T* pointer);

	/**
	 * Constructs a shared pointer from nullptr. The shared pointer is
	 *   initialized to its invalid state.
	 */
	SharedPtr(std::nullptr_t);

	/**
	 * Destructor.
	 */
	~SharedPtr();

	/**
	 * Copy assignment operator.
	 * 
	 * @param other The shared pointer to copy.
	 * 
	 * @return Reference to this, after the copy.
	 */
	SharedPtr<T>& operator=(const SharedPtr<T>& other);

	/**
	 * Move assignment operator.
	 *
	 * @param other The shared pointer to move.
	 *
	 * @return Reference to this, after the move.
	 */
	SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;

	/**
	 * Assignment operator using nullptr. The shared pointer is
	 *   set to its invalid state.
	 * 
	 * @return Reference to this, after the invalidation.
	 */
	SharedPtr<T>& operator=(std::nullptr_t);

public:
	/** @return The held object. */
	ALWAYS_INLINE T* operator->();

	/** @return The held object. */
	ALWAYS_INLINE const T* operator->() const;

public:
	/** @return The held object. */
	ALWAYS_INLINE T* Get();

	/** @return The held object. */
	ALWAYS_INLINE const T* Get() const;

	/** @return True if the pointer is valid (not nullptr); False otherwise. */
	ALWAYS_INLINE bool IsValid() const;

public:
	/**
	 * Equality check operator.
	 * 
	 * @param other The shared pointer to compare against.
	 * 
	 * @return True if the pointers are equal; False otherwise.
	 */
	bool operator==(const SharedPtr<T>& other) const;

	/**
	 * Inequality check operator.
	 *
	 * @param other The shared pointer to compare against.
	 *
	 * @return True if the pointers are not equal; False otherwise.
	 */
	bool operator!=(const SharedPtr<T>& other) const;

	/** @return True if the shared pointer is valid; False otherwise. */
	operator bool() const;

public:
	/**
	 * Releases the object, decrementing its reference count. If the count hits 0,
	 *   the object destructor is called and the memory is freed.
	 */
	void Release();

private:
	// The held object.
	T* m_Pointer;
};

//////////////// SHARED POINTER IMPLEMENTATION ////////////////

template<typename T>
SharedPtr<T>::SharedPtr()
	: m_Pointer(nullptr)
{}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
	: m_Pointer(other.m_Pointer)
{
	if (m_Pointer)
	{
		m_Pointer->__m_ReferenceCount++;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept
	: m_Pointer(other.m_Pointer)
{
	other.m_Pointer = nullptr;
}

template<typename T>
SharedPtr<T>::SharedPtr(T* pointer)
	: m_Pointer(pointer)
{
	if (m_Pointer)
	{
		m_Pointer->__m_ReferenceCount++;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(std::nullptr_t)
	: m_Pointer(nullptr)
{}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
	Release();
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	if (other.m_Pointer)
	{
		other.m_Pointer->__m_ReferenceCount++;
	}

	Release();
	m_Pointer = other.m_Pointer;

	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept
{
	m_Pointer = other.m_Pointer;
	other.m_Pointer = nullptr;
	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(std::nullptr_t)
{
	Release();
	return *this;
}

template<typename T>
ALWAYS_INLINE T* SharedPtr<T>::operator->()
{
	return Get();
}

template<typename T>
ALWAYS_INLINE const T* SharedPtr<T>::operator->() const
{
	return Get();
}

template<typename T>
ALWAYS_INLINE T* SharedPtr<T>::Get()
{
	HC_ASSERT(m_Pointer != nullptr); // Trying to dereference a nullptr!
	return m_Pointer;
}

template<typename T>
ALWAYS_INLINE const T* SharedPtr<T>::Get() const
{
	HC_ASSERT(m_Pointer != nullptr); // Trying to dereference a nullptr!
	return m_Pointer;
}

template<typename T>
ALWAYS_INLINE bool SharedPtr<T>::IsValid() const
{
	return (m_Pointer != nullptr);
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const
{
	return m_Pointer == other.m_Pointer;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const
{
	return m_Pointer != other.m_Pointer;
}

template<typename T>
SharedPtr<T>::operator bool() const
{
	return IsValid();
}

template<typename T>
void SharedPtr<T>::Release()
{
	if (!m_Pointer)
	{
		return;
	}

	if ((--(m_Pointer->__m_ReferenceCount)) == 0)
	{
		HcDelete m_Pointer;
	}

	m_Pointer = nullptr;
}

} // namespace HC