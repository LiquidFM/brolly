/**
 * This file is part of the Brolly library.
 *
 * Copyright (C) 2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * the Brolly library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the Brolly library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Brolly library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../brolly_Assert.h"

#include <platform/platform.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if PLATFORM_OS(DARWIN) || PLATFORM_OS(LINUX)
#include <execinfo.h>
#if PLATFORM_USE(BACKTRACE_SYMBOLS)
#include <cxxabi.h>
#endif
#endif

#if PLATFORM_COMPILER(MSVC) && !PLATFORM_OS(WINCE)
#include <crtdbg.h>
#endif

#if PLATFORM_OS(WINDOWS)
#include <windows.h>

#if PLATFORM_USE(BACKTRACE_SYMBOLS)
#error Backtrace symbols are not supported on Windows platform
#endif

#endif



namespace Brolly {

/* Logging functions can process %@ in the format string to log a NSObject* but the printf format attribute
   emits a warning when %@ is used in the format string.  Until <rdar://problem/5195437> is resolved we can't include
   the attribute when being used from Objective-C code in case it decides to use %@. */
#if PLATFORM_COMPILER(GCC) && !defined(__OBJC__)
#define ATTRIBUTE_PRINTF(formatStringArgument, extraArguments) __attribute__((__format__(printf, formatStringArgument, extraArguments)))
#else
#define ATTRIBUTE_PRINTF(formatStringArgument, extraArguments)
#endif


ATTRIBUTE_PRINTF(1, 0)
static void vprintf_stderr_common(const char* format, va_list args)
{
    vfprintf(stderr, format, args);
}

ATTRIBUTE_PRINTF(1, 2)
static void printf_stderr_common(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf_stderr_common(format, args);
    va_end(args);
}

static void printCallSite(const char* file, int line, const char* function)
{
#if PLATFORM_OS(WINDOWS) && !PLATFORM_OS(WINCE) && defined(_DEBUG)
    _CrtDbgReport(_CRT_WARN, file, line, NULL, "%s\n", function);
#else
    // By using this format, which matches the format used by MSVC for compiler errors, developers
    // using Visual Studio can double-click the file/line number in the Output Window to have the
    // editor navigate to that line of code. It seems fine for other developers, too.
    printf_stderr_common("%s(%d) : %s\n", file, line, function);
#endif
}


static void GetBacktrace(void** stack, int* size)
{
#if PLATFORM_OS(DARWIN) || PLATFORM_OS(LINUX)
    *size = backtrace(stack, *size);
#elif PLATFORM_OS(WINDOWS) && !PLATFORM_OS(WINCE)
    // The CaptureStackBackTrace function is available in XP, but it is not defined
    // in the Windows Server 2003 R2 Platform SDK. So, we'll grab the function
    // through GetProcAddress.
    typedef WORD (NTAPI* RtlCaptureStackBackTraceFunc)(DWORD, DWORD, PVOID*, PDWORD);
    HMODULE kernel32 = ::GetModuleHandleW(L"Kernel32.dll");
    if (!kernel32) {
        *size = 0;
        return;
    }
    RtlCaptureStackBackTraceFunc captureStackBackTraceFunc = reinterpret_cast<RtlCaptureStackBackTraceFunc>(
        ::GetProcAddress(kernel32, "RtlCaptureStackBackTrace"));
    if (captureStackBackTraceFunc)
        *size = captureStackBackTraceFunc(0, *size, stack, 0);
    else
        *size = 0;
#else
    *size = 0;
#endif
}

static void PrintBacktrace(void** stack, int size)
{
#if PLATFORM_USE(BACKTRACE_SYMBOLS)
    char** symbols = backtrace_symbols(stack, size);
    if (!symbols)
        return;
#endif

    for (int i = 0; i < size; ++i) {
        const char* mangledName = 0;
        char* cxaDemangled = 0;
#if PLATFORM_USE(BACKTRACE_SYMBOLS)
        char *begin = 0;
        char *end = 0;
        mangledName = symbols[i];

        for (char *j = symbols[i]; *j; ++j)
			if (*j == '(')
				begin = j;
			else
				if (*j == '+')
					end = j;

		if (begin && end)
		{
			*begin++ = 0;
			*end = 0;
			cxaDemangled = abi::__cxa_demangle(begin, 0, 0, 0);
		}
#elif PLATFORM_USE(DLADDR)
        Dl_info info;
        if (dladdr(stack[i], &info) && info.dli_sname)
            mangledName = info.dli_sname;
        if (mangledName)
            cxaDemangled = abi::__cxa_demangle(mangledName, 0, 0, 0);
#endif
        const int frameNumber = i + 1;
        if (mangledName || cxaDemangled)
            printf_stderr_common("%-3d %p %s\n", frameNumber, stack[i], cxaDemangled ? cxaDemangled : mangledName);
        else
            printf_stderr_common("%-3d %p\n", frameNumber, stack[i]);
        free(cxaDemangled);
    }

#if PLATFORM_USE(BACKTRACE_SYMBOLS)
    free(symbols);
#endif
}

void ReportBacktrace()
{
    static const int framesToShow = 31;
    static const int framesToSkip = 2;
    void* samples[framesToShow + framesToSkip];
    int frames = framesToShow + framesToSkip;

    GetBacktrace(samples, &frames);
    PrintBacktrace(samples + framesToSkip, frames - framesToSkip);
}

void ReportSegmentationFault(void *address, int ip)
{
    printf_stderr_common("SEGMENTATION FAULT: faulty address is %p, from 0x%x\n", address, ip);
}

void ReportBrollyionFailure(const char* file, int line, const char* function, const char* assertion)
{
    if (assertion)
        printf_stderr_common("ASSERTION FAILED: %s\nat\t%s:%i\n", assertion, file, line);
    else
        printf_stderr_common("SHOULD NEVER BE REACHED\n");

    printCallSite(file, line, function);
}

void ReportBrollyionFailureWithMessage(const char* file, int line, const char* function, const char *message)
{
	printf_stderr_common("ASSERTION FAILED: %s\n", message);
    printCallSite(file, line, function);
}

}
