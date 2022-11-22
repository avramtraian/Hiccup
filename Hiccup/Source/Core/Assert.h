// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreDefines.h"
#include "Core/Logger.h"

#if HC_CONFIGURATION_DEBUG
	#define HC_ENABLE_ASSERTS           1
	#define HC_ENABLE_VERIFIES          1
	#define HC_ENABLE_DEBUG_ASSERTS     1
	#define HC_ENABLE_DEBUG_VERIFIES    1
#elif HC_CONFIGURATION_RELEASE
	#define HC_ENABLE_ASSERTS           1
	#define HC_ENABLE_VERIFIES          1
	#define HC_ENABLE_DEBUG_ASSERTS     0
	#define HC_ENABLE_DEBUG_VERIFIES    0
#elif HC_CONFIGURATION_SHIPPING
	#define HC_ENABLE_ASSERTS           0
	#define HC_ENABLE_VERIFIES          0
	#define HC_ENABLE_DEBUG_ASSERTS     0
	#define HC_ENABLE_DEBUG_VERIFIES    0
#endif // Build configuration switch.

namespace HC
{

HC_API void OnAssertFailed(const char* expression, const char* category, const char* fileName, const char* functionName, uint32 lineNumber, const char* message, ...);

} // namespace HC

#if HC_ENABLE_ASSERTS
	#define HC_ASSERT(EXPRESSION)               \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "ASSERT",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				nullptr                         \
			);                                  \
			HC_DEBUGBREAK;                      \
		}

	#define HC_ASSERTF(EXPRESSION, ...)         \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "ASSERT",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				__VA_ARGS__                     \
			);                                  \
			HC_DEBUGBREAK;                      \
		}
#else
	#define HC_ASSERT(EXPRESSION)
	#define HC_ASSERTF(EXPRESSION, ...)
#endif // HC_ENABLE_ASSERTS

#if HC_ENABLE_DEBUG_ASSERTS
	#define HC_DASSERT(EXPRESSION)              \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "ASSERT",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				nullptr                         \
			);                                  \
			HC_DEBUGBREAK;                      \
		}

	#define HC_DASSERTF(EXPRESSION, ...)        \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "ASSERT",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				__VA_ARGS__                     \
			);                                  \
			HC_DEBUGBREAK;                      \
		}
#else
	#define HC_DASSERT(EXPRESSION)
	#define HC_DASSERTF(EXPRESSION, ...)
#endif // HC_ENABLE_DEBUG_ASSERTS

#if HC_ENABLE_VERIFIES
	#define HC_VERIFY(EXPRESSION)               \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "VERIFY",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				nullptr                         \
			);                                  \
			HC_DEBUGBREAK;                      \
		}

	#define HC_VERIFYF(EXPRESSION, ...)         \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "VERIFY",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				__VA_ARGS__                     \
			);                                  \
			HC_DEBUGBREAK;                      \
		}
#else
	#define HC_VERIFY(EXPRESSION) EXPRESSION
	#define HC_VERIFYF(EXPRESSION, ...) EXPRESSION
#endif // HC_ENABLE_VERIFIES

#if HC_ENABLE_DEBUG_VERIFIES
	#define HC_DVERIFY(EXPRESSION)              \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "VERIFY",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				nullptr                         \
			);                                  \
			HC_DEBUGBREAK;                      \
		}

	#define HC_DVERIFYF(EXPRESSION, ...)        \
		if (!(EXPRESSION))                      \
		{                                       \
			::HC::OnAssertFailed(               \
				#EXPRESSION, "VERIFY",          \
				HC_FILE, HC_FUNCTION, HC_LINE,  \
				__VA_ARGS__                     \
			);                                  \
			HC_DEBUGBREAK;                      \
		}
#else
	#define HC_DVERIFY(EXPRESSION) EXPRESSION
	#define HC_DVERIFYF(EXPRESSION, ...) EXPRESSION
#endif // HC_ENABLE_DEBUG_VERIFIES