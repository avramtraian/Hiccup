// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#include "Assert.h"

#include "Memory.h"
#include "Core/Math/MathUtilities.h"
#include "Core/Platform/Platform.h"

#include <cstdarg>
#include <cstdio>

namespace HC
{

void on_assert_failed(const char* expression, const char* category, const char* filename, const char* function_sig, uint32_t line_number, const char* message, ...)
{
    char title_buffer[32];
    uint32_t title_width = (uint32_t)sprintf_s(title_buffer, " %s FAILED ", category);

    char message_buffer[512] = {};
    uint32_t message_width = 0;

    char formatted_message[512] = {};

    if (message)
    {
        va_list arg_list;
        va_start(arg_list, message);
        vsprintf_s(formatted_message, message, arg_list);
        message_width = (uint32_t)sprintf_s(message_buffer, "MESSAGE:    %s", formatted_message);
        va_end(arg_list);
    }

    char expression_buffer[512];
    uint32_t expression_width = (uint32_t)sprintf_s(expression_buffer, "EXPRESSION: %s", expression);

    char file_buffer[512];
    uint32_t file_width = (uint32_t)sprintf_s(file_buffer, "FILE:       %s", filename);

    char function_buffer[512];
    uint32_t function_width = (uint32_t)sprintf_s(function_buffer, "FUNCTION:   %s", function_sig);

    char line_buffer[512];
    uint32_t line_width = (uint32_t)sprintf_s(line_buffer, "LINE:       %u", line_number);

    uint32_t max_width = 0;
    max_width = Math::max(max_width, title_width);
    max_width = Math::max(max_width, expression_width);
    max_width = Math::max(max_width, message_width);
    max_width = Math::max(max_width, file_width);
    max_width = Math::max(max_width, function_width);
    max_width = Math::max(max_width, line_width);

    char whitespace[512];
    Memory::set(whitespace, ' ', sizeof(whitespace));

    // Header.
    {
        const uint32_t total_padding = max_width - title_width;
        const uint32_t left_padding = total_padding / 2;
        const uint32_t right_padding = total_padding / 2 + total_padding % 2;

        char left_padding_buffer[256];
        Memory::set(left_padding_buffer, '-', sizeof(left_padding_buffer));
        left_padding_buffer[left_padding] = 0;

        char right_padding_buffer[256];
        Memory::set(right_padding_buffer, '-', sizeof(right_padding_buffer));
        right_padding_buffer[right_padding] = 0;

        HC_LOG_FATAL("+-%s%s%s-+", left_padding_buffer, title_buffer, right_padding_buffer);
    }

    // Expression.
    {
        const uint32_t whitespace_width = max_width - expression_width;
        whitespace[whitespace_width] = 0;
        HC_LOG_FATAL("| %s%s |", expression_buffer, whitespace);
        whitespace[whitespace_width] = ' ';
    }

    // Message (if any).
    if (message)
    {
        const uint32_t whitespace_width = max_width - message_width;
        whitespace[whitespace_width] = 0;
        HC_LOG_FATAL("| %s%s |", message_buffer, whitespace);
        whitespace[whitespace_width] = ' ';
    }

    // File name.
    {
        const uint32_t whitespace_width = max_width - file_width;
        whitespace[whitespace_width] = 0;
        HC_LOG_FATAL("| %s%s |", file_buffer, whitespace);
        whitespace[whitespace_width] = ' ';
    }

    // Function name.
    {
        const uint32_t whitespace_width = max_width - function_width;
        whitespace[whitespace_width] = 0;
        HC_LOG_FATAL("| %s%s |", function_buffer, whitespace);
        whitespace[whitespace_width] = ' ';
    }

    // Line number.
    {
        const uint32_t whitespace_width = max_width - line_width;
        whitespace[whitespace_width] = 0;
        HC_LOG_FATAL("| %s%s |", line_buffer, whitespace);
        whitespace[whitespace_width] = ' ';
    }

    // Footer.
    {
        char footer_buffer[512];
        Memory::set(footer_buffer, '-', sizeof(footer_buffer));
        footer_buffer[max_width] = 0;
        HC_LOG_FATAL("+-%s-+", footer_buffer);
    }
    
    char buffer[2048];
    sprintf_s(buffer, "Hiccup has crashed!\n\n[Expression]: %s\n\n[Message]: %s\n\n[File]: %s\n\n[Function]: %s\n\n[Line]: %u",
        expression, formatted_message,
        filename, function_sig, line_number);

    Platform::open_popup("C++ Hiccup Assertion Failed", buffer, Platform::POPUP_FLAG_BUTTON_OK | Platform::POPUP_FLAG_ICON_ERROR);
}

} // namespace HC