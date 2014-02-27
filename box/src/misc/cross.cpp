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

#include <string>
#include <stdlib.h>
#include "dosbox.h"
#include "cross.h"
#include "support.h"
//#include <sys/types.h>
//#include <pwd.h>

namespace dosbox {

//void Cross::ResolveHomedir(std::string & temp_line)
//{
//	LOG_MSG("ResolveHomedir()");
//	if(!temp_line.size() || temp_line[0] != '~') return; //No ~
//
//	if(temp_line.size() == 1 || temp_line[1] == CROSS_FILESPLIT) { //The ~ and ~/ variant
//		char * home = getenv("HOME");
//		if(home) temp_line.replace(0,1,std::string(home));
//#if defined HAVE_SYS_TYPES_H && defined HAVE_PWD_H
//	} else { // The ~username variant
//		std::string::size_type namelen = temp_line.find(CROSS_FILESPLIT);
//		if(namelen == std::string::npos) namelen = temp_line.size();
//		std::string username = temp_line.substr(1,namelen - 1);
//		struct passwd* pass = getpwnam(username.c_str());
//		if(pass) temp_line.replace(0,namelen,pass->pw_dir); //namelen -1 +1(for the ~)
//#endif // USERNAME lookup code
//	}
//}

//void Cross::CreateDir(std::string const& in)
//{
//	LOG_MSG("CreateDir()");
//	mkdir(in.c_str(),0700);
//}
//
//bool Cross::IsPathAbsolute(std::string const& in)
//{
//	LOG_MSG("IsPathAbsolute()");
//	if (in.size() > 1 && in[0] == '/' ) return true;
//	return false;
//}

dir_information* open_directory(const char* dirname)
{
	LOG_MSG("open_directory()");
	static dir_information dir;
	dir.dir=opendir(dirname);
	safe_strncpy(dir.base_path,dirname,CROSS_LEN);
	return dir.dir?&dir:NULL;
}

bool read_directory_first(dir_information* dirp, char* entry_name, bool& is_directory)
{
	LOG_MSG("read_directory_first()");
	struct dirent* dentry = readdir(dirp->dir);
	if (dentry==NULL) {
		return false;
	}

//	safe_strncpy(entry_name,dentry->d_name,(FILENAME_MAX<MAX_PATH)?FILENAME_MAX:MAX_PATH);	// [include stdio.h], maybe pathconf()
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

bool read_directory_next(dir_information* dirp, char* entry_name, bool& is_directory)
{
	LOG_MSG("read_directory_next()");
	struct dirent* dentry = readdir(dirp->dir);
	if (dentry==NULL) {
		return false;
	}

//	safe_strncpy(entry_name,dentry->d_name,(FILENAME_MAX<MAX_PATH)?FILENAME_MAX:MAX_PATH);	// [include stdio.h], maybe pathconf()
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
