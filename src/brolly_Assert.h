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

#ifndef BROLLY_BROLLY_H_
#define BROLLY_BROLLY_H_

#include <stdint.h>
#include <platform/utils.h>


namespace Brolly {

void ReportBacktrace();
void ReportSegmentationFault(void *address, int ip);
void ReportBrollyionFailure(const char* file, int line, const char* function, const char* assertion);
void ReportBrollyionFailureWithMessage(const char* file, int line, const char* function, const char *message);

}


/* CRASH() - Raises a fatal error resulting in program termination and triggering either the debugger or the crash reporter.

   Use CRASH() in response to known, unrecoverable errors like out-of-memory.
   Macro is enabled in both debug and release mode.
   To test for unknown errors and verify assumptions, use ASSERT instead, to avoid impacting performance in release builds.

   Signals are ignored by the crash reporter on OS X so we must do better.
*/
#ifndef CRASH
#if PLATFORM_COMPILER(CLANG)
#define CRASH() do { \
    ::Brolly::ReportBacktrace(); \
    *(int *)(uintptr_t)0xbbadbeef = 0; \
    __builtin_trap(); \
} while (false)
#else
#define CRASH() do { \
    ::Brolly::ReportBacktrace(); \
    *(int *)(uintptr_t)0xbbadbeef = 0; \
    ((void(*)())0)(); /* More reliable, but doesn't say BBADBEEF */ \
} while (false)
#endif
#endif


#define ASSERT(assertion) do \
    if (!(assertion)) { \
        ::Brolly::ReportBrollyionFailure(__FILE__, __LINE__, PLATFORM_PRETTY_FUNCTION, #assertion); \
        CRASH(); \
    } \
while (0)


#if PLATFORM_COMPILER_SUPPORTS(CXX_STATIC_ASSERT)
	#define STATIC_ASSERT(Expression, Message) \
		static_assert(Expression, Message)
#else
	#define STATIC_ASSERT(Expression, Message) \
			static const char __STATIC_BROLLY__dummy__[(Expression) ? 1 : -1]
#endif

#endif /* BROLLY_BROLLY_H_ */
