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

#include <string>
#include <stdlib.h>
#include "dosbox.h"
#include "cross.h"
#include "support.h"
#include "ldb.h"

namespace dosbox {

dir_information* open_directory(const char* dirname)
{
#ifndef LDB_SUPPORT_DIRS
	LOG_MSG("open_directory(): STUB");
	return NULL;
#else
	static dir_information dir;
	dir.dir=opendir(dirname);
	safe_strncpy(dir.base_path,dirname,CROSS_LEN);
	return dir.dir?&dir:NULL;
#endif
}

bool read_directory_first(dir_information* dirp, char* entry_name, bool& is_directory)
{
	LOG_MSG("read_directory_first()");
	struct dirent* dentry = readdir(dirp->dir);
	if (dentry==NULL) {
		return false;
	}
	safe_strncpy(entry_name,dentry->d_name,CROSS_LEN);
#ifdef DIRENT_HAS_D_TYPE
	if(dentry->d_type == DT_DIR) {
		is_directory = true;
		return true;
	} else if(dentry->d_type == DT_REG) {
		is_directory = false;
		return true;
	}
#endif
	// probably use d_type here instead of a full stat()
	static char buffer[2*CROSS_LEN] = { 0 };
	buffer[0] = 0;
	strcpy(buffer,dirp->base_path);
	strcat(buffer,entry_name);
	struct stat status;
	if (stat(buffer,&status)==0) is_directory = (S_ISDIR(status.st_mode)>0);
	else is_directory = false;
	return true;
}

void close_directory(dir_information* dirp)
{
	LOG_MSG("close_directory()");
	closedir(dirp->dir);
}

} // namespace dosbox
