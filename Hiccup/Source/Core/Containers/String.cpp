// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#include "String.h"

namespace HC
{

size_t utf8_string_bytes_count(const char* utf8_string)
{
    const char* s = utf8_string;
    while (*(utf8_string++));
    return utf8_string - s - 1;
}

size_t utf8_string_length(const char* utf8_string)
{
    size_t length = 0;

    for (size_t i = 0; i < static_cast<size_t>(-1); ++i, ++length)
    {
        int32_t codepoint = (int32_t)utf8_string[i];
        if (codepoint == 0)
        {
            break;
        }

        // 2-byte character.
        if ((codepoint & 0xE0) == 0xC0)
        {
            i += 1;
        }

        // 3-byte character.
        else if ((codepoint & 0xF0) == 0xE0)
        {
            i += 2;
        }

        // 4-byte character.
        else if ((codepoint & 0xF8) == 0xF0)
        {
            i += 3;
        }
    }

    return length;
}

bool utf8_string_equals(const char* utf8_string_a, const char* utf8_string_b)
{
    while (*utf8_string_a && *utf8_string_b)
    {
        if (*utf8_string_a != *utf8_string_b)
        {
            return false;
        }

        utf8_string_a++;
        utf8_string_b++;
    }

    return (*utf8_string_a == *utf8_string_b);
}

} // namespace HC