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

#include "xsupport.h"

void xnfo(int ctx, char const* format,...)
{
	char buf[256];
	char buff[320];
	va_list msg;
	va_start(msg,format);
	vsnprintf(buf,255,format,msg);
	va_end(msg);
	ctx = ((ctx < 1) || (ctx >= XS_CONTEXT_COUNT))? 0:ctx;
	sprintf(buff,"[%s]: ",xs_contexts[ctx]);
	strcat(buff,buf);
	fprintf(stderr,"%s\n",buff);
}
