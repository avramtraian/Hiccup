// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Memory/Memory.h"

namespace HC
{

template<typename T>
class UniquePtr
{
public:
	HC_NON_COPIABLE(UniquePtr)

	UniquePtr()
		: m_instance(nullptr)
	{}

	UniquePtr(UniquePtr<T>&& other) noexcept
		: m_instance(other.m_instance)
	{
		other.m_instance = 0;
	}

	UniquePtr(T* instance)
		: m_instance(instance)
	{}

	UniquePtr(std::nullptr_t)
		: m_instance(nullptr)
	{}

	~UniquePtr()
	{
		release();
	}

public:
	UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept
	{
		// Self assignment.
		if (this == &other)
		{
			return *this;
		}

		release();
		m_instance = other.m_instance;
		other.m_instance = nullptr;

		return *this;
	}

	UniquePtr<T>& operator=(std::nullptr_t)
	{
		release();
		return *this;
	}

	ALWAYS_INLINE T* operator->() { return get(); }
	ALWAYS_INLINE const T* operator->() const { return get(); }

	ALWAYS_INLINE T& operator*() { return *get(); }
	ALWAYS_INLINE const T& operator*() const { return *get(); }

	ALWAYS_INLINE operator bool() const { return is_valid(); }

public:
	ALWAYS_INLINE T* get()
	{
		HC_DASSERT(m_instance); // Trying to dereference nullptr!
		return m_instance;
	}

	ALWAYS_INLINE const T* get() const
	{
		HC_DASSERT(m_instance); // Trying to dereference nullptr!
		return m_instance;
	}

	ALWAYS_INLINE bool is_valid() const { return (m_instance != nullptr); }

	template<typename Q>
	ALWAYS_INLINE UniquePtr<Q> as()
	{
		Q* instance = (Q*)m_instance;
		m_instance = nullptr;
		return UniquePtr<Q>(instance);
	}

	void release()
	{
		if (m_instance)
		{
			hc_delete m_instance;
			m_instance = nullptr;
		}
	}

public:
	template<typename... Args>
	static UniquePtr<T> create(Args&&... args)
	{
		return UniquePtr<T>(hc_new T(Types::forward<Args>(args)...));
	}

private:
	// The held object instance.
	T* m_instance;
};

} // namespace HC