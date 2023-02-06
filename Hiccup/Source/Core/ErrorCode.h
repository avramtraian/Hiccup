// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#pragma once

#include "Core/CoreTypes.h"

#define HC_CHECK_ERROR(EXPRESSION)              \
    {                                           \
        ::HC::ErrorCode _ec = (EXPRESSION);     \
        if (_ec != ::HC::ErrorCode::Success) {  \
            return _ec;                         \
        }                                       \
    }

namespace HC
{

// This enum contains all the possible error codes any Hiccup system could
// ever use. All functions/utilities that use this method of error checking
// have to respect the following assumptions:
//   1. If a failure is indicated by the error code, the processing result
//      should not be in a usable state.
//   2. By default, only 'Success' is not a failure code. It must be
//      explicitly specified otherwise.
enum class ErrorCode : uint32_t {
    Success = 0,
    KeyAlreadyExists,
};

} // namespace HC
