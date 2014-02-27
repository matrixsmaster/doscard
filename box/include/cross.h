/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include "dosbox.h"

namespace dosbox {

#define LONGTYPE(a) a##LL
#define CROSS_LEN 512						/* Maximum filename size */

#define	CROSS_FILENAME(blah) strreplace(blah,'\\','/')
#define CROSS_FILESPLIT '/'

//#define CROSS_NONE	0
//#define CROSS_FILE	1
//#define CROSS_DIR	2

//class Cross {
//public:
//	static void CreateDir(std::string const& temp);
//	static bool IsPathAbsolute(std::string const& in);
//};

typedef struct dir_struct { 
	DIR*  dir;
	char base_path[CROSS_LEN];
} dir_information;

dir_information* open_directory(const char* dirname);
bool read_directory_first(dir_information* dirp, char* entry_name, bool& is_directory);
bool read_directory_next(dir_information* dirp, char* entry_name, bool& is_directory);
void close_directory(dir_information* dirp);

}

#endif
