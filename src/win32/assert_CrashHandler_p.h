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

#include "../assert_Brolly.h"


namespace Brolly {
static CrashHandler *s_instance;


CrashHandler::CrashHandler()
{
	ASSERT(s_instance == NULL);
	s_instance = this;
}

CrashHandler::~CrashHandler()
{
	ASSERT(s_instance == this);
	s_instance = NULL;
}

}
