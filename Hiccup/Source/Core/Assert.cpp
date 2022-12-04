// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#include "Assert.h"

#include "Memory.h"
#include "Core/Math/MathUtilities.h"
#include "Core/Platform/Platform.h"

#include <cstdarg>
#include <cstdio>

namespace HC
{

void OnAssertFailed(const char* expression, const char* category, const char* fileName, const char* functionName, uint32 lineNumber, const char* message, ...)
{
	char titleBuffer[32];
	uint32 titleWidth = (uint32)sprintf_s(titleBuffer, " %s FAILED ", category);

	char messageBuffer[512] = {};
	uint32 messageWidth = 0;

	char formattedMessage[512] = {};

	if (message)
	{
		va_list argList;
		va_start(argList, message);
		vsprintf_s(formattedMessage, message, argList);
		messageWidth = (uint32)sprintf_s(messageBuffer, "MESSAGE:    %s", formattedMessage);
		va_end(argList);
	}

	char expressionBuffer[512];
	uint32 expressionWidth = (uint32)sprintf_s(expressionBuffer, "EXPRESSION: %s", expression);

	char fileBuffer[512];
	uint32 fileWidth = (uint32)sprintf_s(fileBuffer, "FILE:       %s", fileName);

	char functionBuffer[512];
	uint32 functionWidth = (uint32)sprintf_s(functionBuffer, "FUNCTION:   %s", functionName);

	char lineBuffer[512];
	uint32 lineWidth = (uint32)sprintf_s(lineBuffer, "LINE:       %u", lineNumber);

	uint32 maxWidth = 0;
	maxWidth = Math::Max(maxWidth, titleWidth);
	maxWidth = Math::Max(maxWidth, expressionWidth);
	maxWidth = Math::Max(maxWidth, messageWidth);
	maxWidth = Math::Max(maxWidth, fileWidth);
	maxWidth = Math::Max(maxWidth, functionWidth);
	maxWidth = Math::Max(maxWidth, lineWidth);

	char whitespace[512];
	Memory::set(whitespace, ' ', sizeof(whitespace));

	// Header.
	{
		uint32 totalPadding = maxWidth - titleWidth;
		uint32 leftPadding = totalPadding / 2;
		uint32 rightPadding = totalPadding / 2 + totalPadding % 2;

		char leftPaddingBuffer[256];
		Memory::set(leftPaddingBuffer, '-', sizeof(leftPaddingBuffer));
		leftPaddingBuffer[leftPadding] = 0;

		char rightPaddingBuffer[256];
		Memory::set(rightPaddingBuffer, '-', sizeof(rightPaddingBuffer));
		rightPaddingBuffer[rightPadding] = 0;

		HC_LOG_FATAL("+-%s%s%s-+", leftPaddingBuffer, titleBuffer, rightPaddingBuffer);
	}

	// Expression.
	{
		uint32 whitespaceWidth = maxWidth - expressionWidth;
		whitespace[whitespaceWidth] = 0;
		HC_LOG_FATAL("| %s%s |", expressionBuffer, whitespace);
		whitespace[whitespaceWidth] = ' ';
	}

	// Message (if any).
	if (message)
	{
		uint32 whitespaceWidth = maxWidth - messageWidth;
		whitespace[whitespaceWidth] = 0;
		HC_LOG_FATAL("| %s%s |", messageBuffer, whitespace);
		whitespace[whitespaceWidth] = ' ';
	}

	// File name.
	{
		uint32 whitespaceWidth = maxWidth - fileWidth;
		whitespace[whitespaceWidth] = 0;
		HC_LOG_FATAL("| %s%s |", fileBuffer, whitespace);
		whitespace[whitespaceWidth] = ' ';
	}

	// Function name.
	{
		uint32 whitespaceWidth = maxWidth - functionWidth;
		whitespace[whitespaceWidth] = 0;
		HC_LOG_FATAL("| %s%s |", functionBuffer, whitespace);
		whitespace[whitespaceWidth] = ' ';
	}

	// Line number.
	{
		uint32 whitespaceWidth = maxWidth - lineWidth;
		whitespace[whitespaceWidth] = 0;
		HC_LOG_FATAL("| %s%s |", lineBuffer, whitespace);
		whitespace[whitespaceWidth] = ' ';
	}

	// Footer.
	{
		char footerBuffer[512];
		Memory::set(footerBuffer, '-', sizeof(footerBuffer));
		footerBuffer[maxWidth] = 0;
		HC_LOG_FATAL("+-%s-+", footerBuffer);
	}
	
	char buffer[2048];
	sprintf_s(buffer, "Hiccup has crashed!\n\n[Expression]: %s\n\n[Message]: %s\n\n[File]: %s\n\n[Function]: %s\n\n[Line]: %u",
		expression, formattedMessage,
		fileName, functionName, lineNumber);

	Platform::OpenPopup("C++ Hiccup Assertion Failed", buffer, Platform::POPUP_FLAG_BUTTON_OK | Platform::POPUP_FLAG_ICON_ERROR);
}

} // namespace HC