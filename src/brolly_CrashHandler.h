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

#ifndef BROLLY_CRASHHANDLER_H_
#define BROLLY_CRASHHANDLER_H_

#include <platform/utils.h>


namespace Brolly {

class CrashHandler
{
	PLATFORM_MAKE_NONCOPYABLE(CrashHandler)
	PLATFORM_MAKE_NONMOVEABLE(CrashHandler)
	PLATFORM_MAKE_STACK_ONLY

public:
	CrashHandler();
	~CrashHandler();
};

}

#endif /* BROLLY_CRASHHANDLER_H_ */
