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

#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>

/* get REG_EIP/REG_RIP from ucontext.h */
#ifndef __USE_GNU
#	define __USE_GNU
#endif
#include <ucontext.h>


typedef void (*SignalHandler) (int, siginfo_t *, void *);
SignalHandler default_sighandler;

static void sighandler(int signum, siginfo_t *info, void *ptr)
{
	/* If it wasn't our CRASH() */
	if (info->si_addr != (void *)0xbbadbeef)
	{
		ucontext_t *uc = static_cast<ucontext_t *>(ptr);

#if PLATFORM_CPU(X86)
		::Brolly::ReportSegmentationFault(info->si_addr, uc->uc_mcontext.gregs[REG_EIP]);
#elif PLATFORM_CPU(X86_64)
		::Brolly::ReportSegmentationFault(info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
#endif

		::Brolly::ReportBacktrace();
	}

	default_sighandler(signum, info, ptr);
}


namespace Brolly {
static CrashHandler *s_instance;


CrashHandler::CrashHandler()
{
	ASSERT(s_instance == NULL);
	s_instance = this;

	rlimit core_limit = { RLIM_INFINITY, RLIM_INFINITY };
	ASSERT(setrlimit(RLIMIT_CORE, &core_limit) == 0);

	/* Install our signal handler */
	struct sigaction sa, old;

	sa.sa_sigaction = sighandler;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	ASSERT(sigemptyset(&sa.sa_mask) == 0);

	ASSERT(sigaction(SIGSEGV, &sa, &old) == 0);
	default_sighandler = old.sa_sigaction;
}

CrashHandler::~CrashHandler()
{
	ASSERT(s_instance == this);
	s_instance = NULL;
}

}
