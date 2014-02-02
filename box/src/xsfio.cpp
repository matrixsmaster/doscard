/*
 *  Copyright (C) 2013-2014  Soloviov Dmitry
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

#include <stdio.h>
#include <stdlib.h>
#include "dosbox.h"
#include "ldb.h"

DBFILE* dbfopen(const char* p, const char* m)
{
	DBFILE* r = new DBFILE;
	LOG_MSG("Pretend to open file %s for %s",p,m);
	return r;
}

void dbfclose(DBFILE* f)
{
	if (f) delete f;
}

size_t dbfread(void* p, size_t sz, size_t q, DBFILE* f)
{
	return 0;
}

size_t dbfwrite(const void* p, size_t sz, size_t q, DBFILE* f)
{
	return 0;
}

uint32_t dbfseek(DBFILE* f, uint64_t off, int wh)
{
	return 0;
}

uint64_t dbftell(DBFILE* f)
{
	return 0;
}

int dbfileno(DBFILE* f)
{
	return 0;
}

int dbfeof(DBFILE* f)
{
	return 1;
}

int dbftruncate(DBFILE* f, off_t len)
{
	return 0;
}
