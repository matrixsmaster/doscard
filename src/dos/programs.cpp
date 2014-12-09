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


#include <vector>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "programs.h"
#include "callback.h"
#include "regs.h"
#include "support.h"
#include "cross.h"
#include "shell.h"
#include "ldb.h"

namespace dosbox {

Bitu call_program;

/* This registers a file on the virtual drive and creates the correct structure for it*/

static Bit8u exe_block[]={
	0xbc,0x00,0x04,					//MOV SP,0x400 decrease stack size
	0xbb,0x40,0x00,					//MOV BX,0x040 for memory resize
	0xb4,0x4a,						//MOV AH,0x4A	Resize memory block
	0xcd,0x21,						//INT 0x21
//pos 12 is callback number
	0xFE,0x38,0x00,0x00,			//CALLBack number
	0xb8,0x00,0x4c,					//Mov ax,4c00
	0xcd,0x21,						//INT 0x21
};

#define CB_POS 12

static std::vector<PROGRAMS_Main*> internal_progs;

void PROGRAMS_MakeFile(char const * const name,PROGRAMS_Main * main) {
	Bit8u * comdata=(Bit8u *)malloc(32); //MEM LEAK
	memcpy(comdata,&exe_block,sizeof(exe_block));
	comdata[CB_POS]=(Bit8u)(call_program&0xff);
	comdata[CB_POS+1]=(Bit8u)((call_program>>8)&0xff);

	/* Copy save the pointer in the vector and save it's index */
	if (internal_progs.size()>255) E_Exit("PROGRAMS_MakeFile program size too large (%d)",static_cast<int>(internal_progs.size()));
	Bit8u index = (Bit8u)internal_progs.size();
	internal_progs.push_back(main);

	memcpy(&comdata[sizeof(exe_block)],&index,sizeof(index));
	Bit32u size=sizeof(exe_block)+sizeof(index);	
	VFILE_Register(name,comdata,size);	
}



static Bitu PROGRAMS_Handler(void) {
	/* This sets up everything for a program start up call */
	Bitu size=sizeof(Bit8u);
	Bit8u index;
	/* Read the index from program code in memory */
	PhysPt reader=PhysMake(dos.psp(),256+sizeof(exe_block));
	HostPt writer=(HostPt)&index;
	for (;size>0;size--) *writer++=mem_readb(reader++);
	Program * new_program;
	if (index > internal_progs.size()) E_Exit("something is messing with the memory");
	PROGRAMS_Main * handler = internal_progs[index];
	(*handler)(&new_program);
	new_program->Run();
	delete new_program;
	return CBRET_NONE;
}


/* Main functions used in all program */


Program::Program() {
	/* Find the command line and setup the PSP */
	psp = new DOS_PSP(dos.psp());
	/* Scan environment for filename */
	PhysPt envscan=PhysMake(psp->GetEnvironment(),0);
	while (mem_readb(envscan)) envscan+=mem_strlen(envscan)+1;	
	envscan+=3;
	CommandTail tail;
	MEM_BlockRead(PhysMake(dos.psp(),128),&tail,128);
	if (tail.count<127) tail.buffer[tail.count]=0;
	else tail.buffer[126]=0;
	char filename[256+1];
	MEM_StrCopy(envscan,filename,256);
	cmd = new CommandLine(filename,tail.buffer);
}

extern std::string full_arguments;

void Program::ChangeToLongCmd() {
	/* 
	 * Get arguments directly from the shell instead of the psp.
	 * this is done in securemode: (as then the arguments to mount and friends
	 * can only be given on the shell ( so no int 21 4b) 
	 * Securemode part is disabled as each of the internal command has already
	 * protection for it. (and it breaks games like cdman)
	 * it is also done for long arguments to as it is convient (as the total commandline can be longer then 127 characters.
	 * imgmount with lot's of parameters
	 * Length of arguments can be ~120. but switch when above 100 to be sure
	 */

	if (/*control->SecureMode() ||*/ cmd->Get_arglength() > 100) {	
		CommandLine* temp = new CommandLine(cmd->GetFileName(),full_arguments.c_str());
		delete cmd;
		cmd = temp;
	}
	full_arguments.assign(""); //Clear so it gets even more save
}

static char last_written_character = 0; //For 0xA to OxD 0xA expansion
//The next function didn't break DosBox's original "coding style" at all :)
static void DOS_CON_Write(const char* buf)
{
	Bit16u i,s = 1;
	Bit8u out;
	Bit16u size = (Bit16u)strlen(buf);
	for (i = 0; i < size; i++) {
		if (buf[i] == 0xA && last_written_character != 0xD) {
			out = 0xD;
			DOS_WriteFile(STDOUT,&out,&s);
		}
		last_written_character = out = buf[i];
		DOS_WriteFile(STDOUT,&out,&s);
	}
}

void Program::WriteOut(const char * format,...)
{
	char buf[2048];
	va_list msg;

	va_start(msg,format);
	vsnprintf(buf,2047,format,msg);
	va_end(msg);

	DOS_CON_Write(buf);
}

void Program::WriteOut_NoParsing(const char * format)
{
	DOS_CON_Write(format);
}

bool Program::GetEnvStr(const char * entry,std::string & result) {
	/* Walk through the internal environment and see for a match */
	PhysPt env_read=PhysMake(psp->GetEnvironment(),0);

	char env_string[1024+1];
	result.erase();
	if (!entry[0]) return false;
	do 	{
		MEM_StrCopy(env_read,env_string,1024);
		if (!env_string[0]) return false;
		env_read += (PhysPt)(strlen(env_string)+1);
		char* equal = strchr(env_string,'=');
		if (!equal) continue;
		/* replace the = with \0 to get the length */
		*equal = 0;
		if (strlen(env_string) != strlen(entry)) continue;
		if (strcasecmp(entry,env_string)!=0) continue;
		/* restore the = to get the original result */
		*equal = '=';
		result = env_string;
		return true;
	} while (1);
	return false;
}

bool Program::GetEnvNum(Bitu num,std::string & result) {
	char env_string[1024+1];
	PhysPt env_read=PhysMake(psp->GetEnvironment(),0);
	do 	{
		MEM_StrCopy(env_read,env_string,1024);
		if (!env_string[0]) break;
		if (!num) { result=env_string;return true;}
		env_read += (PhysPt)(strlen(env_string)+1);
		num--;
	} while (1);
	return false;
}

Bitu Program::GetEnvCount(void) {
	PhysPt env_read=PhysMake(psp->GetEnvironment(),0);
	Bitu num=0;
	while (mem_readb(env_read)!=0) {
		for (;mem_readb(env_read);env_read++) {};
		env_read++;
		num++;
	}
	return num;
}

bool Program::SetEnv(const char * entry,const char * new_string) {
	PhysPt env_read=PhysMake(psp->GetEnvironment(),0);
	PhysPt env_write=env_read;
	char env_string[1024+1];
	do 	{
		MEM_StrCopy(env_read,env_string,1024);
		if (!env_string[0]) break;
		env_read += (PhysPt)(strlen(env_string)+1);
		if (!strchr(env_string,'=')) continue;		/* Remove corrupt entry? */
		if ((strncasecmp(entry,env_string,strlen(entry))==0) && 
			env_string[strlen(entry)]=='=') continue;
		MEM_BlockWrite(env_write,env_string,(Bitu)(strlen(env_string)+1));
		env_write += (PhysPt)(strlen(env_string)+1);
	} while (1);
/* TODO Maybe save the program name sometime. not really needed though */
	/* Save the new entry */
	if (new_string[0]) {
		std::string bigentry(entry);
		for (std::string::iterator it = bigentry.begin(); it != bigentry.end(); ++it) *it = toupper(*it);
		sprintf(env_string,"%s=%s",bigentry.c_str(),new_string); 
//		sprintf(env_string,"%s=%s",entry,new_string); //oldcode
		MEM_BlockWrite(env_write,env_string,(Bitu)(strlen(env_string)+1));
		env_write += (PhysPt)(strlen(env_string)+1);
	}
	/* Clear out the final piece of the environment */
	mem_writed(env_write,0);
	return true;
}

bool CommandLine::FindExist(char const * const name,bool remove) {
	cmd_it it;
	if (!(FindEntry(name,it,false))) return false;
	if (remove) cmds.erase(it);
	return true;
}

bool CommandLine::FindHex(char const * const name,int & value,bool remove) {
	cmd_it it,it_next;
	if (!(FindEntry(name,it,true))) return false;
	it_next=it;it_next++;
	sscanf((*it_next).c_str(),"%X",&value);
	if (remove) cmds.erase(it,++it_next);
	return true;
}

bool CommandLine::FindInt(char const * const name,int & value,bool remove) {
	cmd_it it,it_next;
	if (!(FindEntry(name,it,true))) return false;
	it_next=it;it_next++;
	value=atoi((*it_next).c_str());
	if (remove) cmds.erase(it,++it_next);
	return true;
}

bool CommandLine::FindString(char const * const name,std::string & value,bool remove) {
	cmd_it it,it_next;
	if (!(FindEntry(name,it,true))) return false;
	it_next=it;it_next++;
	value=*it_next;
	if (remove) cmds.erase(it,++it_next);
	return true;
}

bool CommandLine::FindCommand(unsigned int which,std::string & value) {
	if (which<1) return false;
	if (which>cmds.size()) return false;
	cmd_it it=cmds.begin();
	for (;which>1;which--) it++;
	value=(*it);
	return true;
}

bool CommandLine::FindEntry(char const * const name,cmd_it & it,bool neednext) {
	for (it=cmds.begin();it!=cmds.end();it++) {
		if (!strcasecmp((*it).c_str(),name)) {
			cmd_it itnext=it;itnext++;
			if (neednext && (itnext==cmds.end())) return false;
			return true;
		}
	}
	return false;
}

bool CommandLine::FindStringBegin(char const* const begin,std::string & value, bool remove) {
	size_t len = strlen(begin);
	for (cmd_it it=cmds.begin();it!=cmds.end();it++) {
		if (strncmp(begin,(*it).c_str(),len)==0) {
			value=((*it).c_str() + len);
			if (remove) cmds.erase(it);
			return true;
		}
	}
	return false;
}

bool CommandLine::FindStringRemain(char const * const name,std::string & value) {
	cmd_it it;value="";
	if (!FindEntry(name,it)) return false;
	it++;
	for (;it!=cmds.end();it++) {
		value+=" ";
		value+=(*it);
	}
	return true;
}

/* Only used for parsing command.com /C
 * Allowing /C dir and /Cdir
 * Restoring quotes back into the commands so command /C mount d "/tmp/a b" works as intended
 */
bool CommandLine::FindStringRemainBegin(char const * const name,std::string & value) {
	cmd_it it;value="";
	if (!FindEntry(name,it)) {
		size_t len = strlen(name);
		for (it=cmds.begin();it!=cmds.end();it++) {
			if (strncasecmp(name,(*it).c_str(),len)==0) {
				std::string temp = ((*it).c_str() + len);
				//Restore quotes for correct parsing in later stages
				if(temp.find(" ") != std::string::npos)
					value = std::string("\"") + temp + std::string("\"");
				else
					value = temp;
				break;
			}
		}
		if( it == cmds.end()) return false;
	}
	it++;
	for (;it!=cmds.end();it++) {
		value += " ";
		std::string temp = (*it);
		if(temp.find(" ") != std::string::npos)
			value += std::string("\"") + temp + std::string("\"");
		else
			value += temp;
	}
	return true;
}

bool CommandLine::GetStringRemain(std::string & value) {
	if(!cmds.size()) return false;

	cmd_it it=cmds.begin();value=(*it++);
	for(;it != cmds.end();it++) {
		value+=" ";
		value+=(*it);
	}
	return true;
}


unsigned int CommandLine::GetCount(void) {
	return (unsigned int)cmds.size();
}

void CommandLine::FillVector(std::vector<std::string> & vector) {
	for(cmd_it it=cmds.begin(); it != cmds.end(); it++) {
		vector.push_back((*it));
	}
	// add back the \" if the parameter contained a space
	for(Bitu i = 0; i < vector.size(); i++) {
		if(vector[i].find(' ') != std::string::npos) {
			vector[i] = "\""+vector[i]+"\"";
		}
	}
}

int CommandLine::GetParameterFromList(const char* const params[], std::vector<std::string> & output) {
	// return values: 0 = P_NOMATCH, 1 = P_NOPARAMS
	// TODO return nomoreparams
	int retval = 1;
	output.clear();
	enum {
		P_START, P_FIRSTNOMATCH, P_FIRSTMATCH
	} parsestate = P_START;
	cmd_it it = cmds.begin();
	while(it!=cmds.end()) {
		bool found = false;
		for(Bitu i = 0; *params[i]!=0; i++) {
			if (!strcasecmp((*it).c_str(),params[i])) {
				// found a parameter
				found = true;
				switch(parsestate) {
				case P_START:
					retval = i+2;
					parsestate = P_FIRSTMATCH;
					break;
				case P_FIRSTMATCH:
				case P_FIRSTNOMATCH:
					return retval;
				}
			}
		}
		if(!found)
			switch(parsestate) {
			case P_START:
				retval = 0; // no match
				parsestate = P_FIRSTNOMATCH;
				output.push_back(*it);
				break;
			case P_FIRSTMATCH:
			case P_FIRSTNOMATCH:
				output.push_back(*it);
				break;
			}
		cmd_it itold = it;
		it++;
		cmds.erase(itold);

	}
	return retval;
}


CommandLine::CommandLine(int argc,char const * const argv[]) {
	if (argc>0) {
		file_name=argv[0];
	}
	int i=1;
	while (i<argc) {
		cmds.push_back(argv[i]);
		i++;
	}
}
Bit16u CommandLine::Get_arglength() {
	if(cmds.empty()) return 0;
	Bit16u i=1;
	for(cmd_it it=cmds.begin();it != cmds.end();it++)
		i+=(*it).size() + 1;
	return --i;
}


CommandLine::CommandLine(char const * const name,char const * const cmdline) {
	if (name) file_name=name;
	/* Parse the cmds and put them in the list */
	bool inword,inquote;char c;
	inword=false;inquote=false;
	std::string str;
	const char * c_cmdline=cmdline;
	while ((c=*c_cmdline)!=0) {
		if (inquote) {
			if (c!='"') str+=c;
			else {
				inquote=false;
				cmds.push_back(str);
				str.erase();
			}
		}else if (inword) {
			if (c!=' ') str+=c;
			else {
				inword=false;
				cmds.push_back(str);
				str.erase();
			}
		}
		else if (c=='"') { inquote=true;}
		else if (c!=' ') { str+=c;inword=true;}
		c_cmdline++;
	}
	if (inword || inquote) cmds.push_back(str);
}

void CommandLine::Shift(unsigned int amount) {
	while(amount--) {
		file_name = cmds.size()?(*(cmds.begin())):"";
		if(cmds.size()) cmds.erase(cmds.begin());
	}
}

void PROGRAMS_Init() {
	/* Setup a special callback to start virtual programs */
	call_program=CALLBACK_Allocate();
	CALLBACK_Setup(call_program,&PROGRAMS_Handler,CB_RETF,"internal program");
//	PROGRAMS_MakeFile("CONFIG.COM",CONFIG_ProgramStart);
}

}
