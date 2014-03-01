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


#include <stdlib.h>
#include <string.h>
#include "dosbox.h"
#include "cross.h"
#include "support.h"
#include <list>
#include <string>

using namespace std;

namespace dosbox {

#define LINE_IN_MAXLEN 2048

struct MessageBlock {
	string name;
	string val;
	MessageBlock(const char* _name, const char* _val):
		name(_name),val(_val){}
};

static list<MessageBlock> Lang;
typedef list<MessageBlock>::iterator itmb;

void MSG_Add(const char * _name, const char* _val)
{
	/* Find the message */
	for(itmb tel=Lang.begin();tel!=Lang.end();tel++) {
		if((*tel).name==_name) { 
			//			LOG_MSG("double entry for %s",_name); //Message file might be loaded before default text messages
			return;
		}
	}
	/* if the message doesn't exist add it */
	Lang.push_back(MessageBlock(_name,_val));
}

void MSG_Replace(const char * _name, const char* _val)
{
	/* Find the message */
	for(itmb tel=Lang.begin();tel!=Lang.end();tel++) {
		if((*tel).name==_name) { 
			Lang.erase(tel);
			break;
		}
	}
	/* Even if the message doesn't exist add it */
	Lang.push_back(MessageBlock(_name,_val));
}

const char * MSG_Get(char const * msg)
{
	for(itmb tel=Lang.begin();tel!=Lang.end();tel++){	
		if((*tel).name==msg)
		{
			return  (*tel).val.c_str();
		}
	}
	return "Message not Found!\n";
}

bool MSG_Write(const char * location)
{
	DBFILE* out= dbfopen(location,"w+t");
	if(out==NULL) return false;//maybe an error?
	for(itmb tel=Lang.begin();tel!=Lang.end();tel++)
		dbfprintf(out,":%s\n%s\n.\n",(*tel).name.c_str(),(*tel).val.c_str());
	dbfclose(out);
	return true;
}

}
