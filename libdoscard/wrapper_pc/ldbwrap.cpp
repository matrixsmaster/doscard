/*
 *  Copyright (C) 2014  Soloviov Dmitry
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "ldbwrap.h"

using namespace dosbox;

namespace doscard {

int CreateInstance(LDB_Settings* set)
{
	return -1;
}

int TryDestroyInstance(void)
{
	return -1;
}

int RunInstance(void)
{
	return -1;
}

int GetInstanceRuntime(void*,uint64_t)
{
	return -1;
}

int GetInstanceScreen(void*,uint64_t)
{
	return -1;
}

int GetInstanceSound(void*,uint64_t)
{
	return -1;
}

int AddInstanceEvents(void*,uint64_t)
{
	return -1;
}

int GetInstanceMessages(void*,uint64_t)
{
	return -1;
}

} //namespace doscard
