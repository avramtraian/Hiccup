// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

//////////////// PLATFORM ////////////////

#ifndef HC_PLATFORM_WINDOWS
	#define HC_PLATFORM_WINDOWS         0
#endif // HC_PLATFORM_WINDOWS

#ifndef HC_PLATFORM_LINUX
	#define HC_PLATFORM_LINUX           0
#endif // HC_PLATFORM_LINUX

#ifndef HC_PLATFORM_MACOS
	#define HC_PLATFORM_MACOS           0
#endif // HC_PLATFORM_MACOS

#if !HC_PLATFORM_WINDOWS && !HC_PLATFORM_LINUX && !HC_PLATFORM_MACOS
	#error Unknown or unsupported platform! Hiccup only supports Windows.
#endif // No platform.

//////////////// IMPORT/EXPORT SPECIFIERS ////////////////

#if HC_PLATFORM_WINDOWS
	#define HC_SPECIFIER_EXPORT         __declspec(dllexport)
	#define HC_SPECIFIER_IMPORT         __declspec(dllimport)
#elif HC_PLATFORM_LINUX
	#define HC_SPECIFIER_EXPORT
	#define HC_SPECIFIER_IMPORT
#elif HC_PLATFORM_MACOS
	#define HC_SPECIFIER_EXPORT
	#define HC_SPECIFIER_IMPORT
#endif // Platform switch.

#ifdef HC_EXPORT_API
	#define HC_API                      HC_SPECIFIER_EXPORT
#else
	#define HC_API                      HC_SPECIFIER_IMPORT
#endif // HC_EXPORT_API

//////////////// BUILD CONFIGURATION ////////////////

#ifndef HC_CONFIGURATION_DEBUG
	#define HC_CONFIGURATION_DEBUG      0
#endif // HC_CONFIGURATION_DEBUG

#ifndef HC_CONFIGURATION_RELEASE
	#define HC_CONFIGURATION_RELEASE    0
#endif // HC_CONFIGURATION_RELEASE

#ifndef HC_CONFIGURATION_SHIPPING
	#define HC_CONFIGURATION_SHIPPING   0
#endif // HC_CONFIGURATION_SHIPPING

#if !HC_CONFIGURATION_DEBUG && !HC_CONFIGURATION_RELEASE && !HC_CONFIGURATION_SHIPPING
	#error Build configuration not specified!
#endif // No build configuration.

//////////////// COMPILER ////////////////

#ifdef _MSC_BUILD
	#define HC_COMPILER_MSVC            1
#endif // _MSC_BUILD

#ifdef __clang__
	#define HC_COMPILER_CLANG           1
	#define HC_COMPILER_GCC_CLANG       1
#endif // __clang__

#ifdef __gcc__
	#define HC_COMPILER_GCC             1
	#define HC_COMPILER_GCC_CLANG       1
#endif // __clang__

#ifndef HC_COMPILER_MSVC
	#define HC_COMPILER_MSVC            0
#endif // HC_COMPILER_MSVC

#ifndef HC_COMPILER_CLANG
	#define HC_COMPILER_CLANG           0
#endif // HC_COMPILER_CLANG

#ifndef HC_COMPILER_GCC
	#define HC_COMPILER_GCC             0
#endif // HC_COMPILER_GCC

#ifndef HC_COMPILER_GCC_CLANG
	#define HC_COMPILER_GCC_CLANG       0
#endif // HC_COMPILER_GCC_CLANG

#if !HC_COMPILER_MSVC && !HC_COMPILER_CLANG && !HC_COMPILER_GCC
	#error Unknown or unsupported compiler! Hiccup only supports MSVC, Clang and GCC.
#endif // No compiler.

//////////////// COMPILER-SPECIFIC UTILITIES ////////////////

#if HC_COMPILER_MSVC
	#define HC_DEBUGBREAK               __debugbreak()
	#define FORCEINLINE                 __forceinline
	#define HC_FUNCTION                 __FUNCSIG__
#elif HC_COMPILER_GCC_CLANG
	#define HC_DEBUGBREAK               __builtin_trap()
	#define FORCEINLINE                 inline
	#define HC_FUNCTION                 __PRETTY_FUNCTION__
#endif // Compiler switch.

//////////////// UTILITIES ////////////////

#define internal                        static
#define persistent                      static

#define ArrayCount(ARRAY)               (sizeof(ARRAY) / sizeof(ARRAY[0]))

#define Bit(X)                          (1 << (X))

#define HC_LINE                         (::HC::uint32)(__LINE__)
#define HC_FILE                         __FILE__
#define HC_DATE                         __DATE__

#define NODISCARD                       [[nodiscard]]
#define MAYBE_UNUSED                    [[maybe_unused]]
#define LIKELY                          [[likely]]
#define UNLIKELY                        [[unlikely]]