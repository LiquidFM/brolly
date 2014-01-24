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

#include "brolly_CrashHandler.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "unix/brolly_CrashHandler_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/brolly_CrashHandler_p.h"
#endif
