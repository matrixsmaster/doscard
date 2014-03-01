/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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

#ifndef DOSBOX_CROSS_H
#define DOSBOX_CROSS_H

#include <string>
#include <unistd.h>
#include "dosbox.h"

//FIXME: remove them
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

namespace dosbox {

#define LONGTYPE(a) a##LL
#define CROSS_LEN 512						/* Maximum filename size */

#define	CROSS_FILENAME(x) strreplace(x,'\\','/')
#define CROSS_FILESPLIT '/'

typedef struct dir_struct { 
	DIR*  dir;
	char base_path[CROSS_LEN];
} dir_information;

dir_information* open_directory(const char* dirname);
bool read_directory_first(dir_information* dirp, char* entry_name, bool& is_directory);
//bool read_directory_next(dir_information* dirp, char* entry_name, bool& is_directory);
#define read_directory_next(x,y,z) read_directory_first(x,y,z)
void close_directory(dir_information* dirp);

}

#endif
