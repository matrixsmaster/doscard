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
#include <stdarg.h>
#include <string.h>
#include "dosbox.h"
#include "messages.h"
#include "regs.h"
#include "shell.h"
#include "callback.h"
#include "support.h"

namespace dosbox {

Bitu call_shellstop;
/* Larger scope so shell_del autoexec can use it to
 * remove things from the environment */
Program * first_shell = 0; 

static Bitu shellstop_handler(void) {
	return CBRET_STOP;
}

static void SHELL_ProgramStart(Program * * make) {
	*make = new DOS_Shell;
}

#define AUTOEXEC_SIZE 4096
static char autoexec_data[AUTOEXEC_SIZE] = { 0 };
static std::list<std::string> autoexec_strings;
typedef std::list<std::string>::iterator auto_it;

void VFILE_Remove(const char *name);

void AutoexecObject::Install(const std::string &in) {
	if(GCC_UNLIKELY(installed)) E_Exit("autoexec: already created %s",buf.c_str());
	installed = true;
	buf = in;
	autoexec_strings.push_back(buf);
	this->CreateAutoexec();

	//autoexec.bat is normally created AUTOEXEC_Init.
	//But if we are already running (first_shell)
	//we have to update the environment to display changes

	if(first_shell)	{
		//create a copy as the string will be modified
		std::string::size_type n = buf.size();
		char* buf2 = new char[n + 1];
		safe_strncpy(buf2, buf.c_str(), n + 1);
		if((strncasecmp(buf2,"set ",4) == 0) && (strlen(buf2) > 4)){
			char* after_set = buf2 + 4;//move to variable that is being set
			char* test = strpbrk(after_set,"=");
			if(!test) {first_shell->SetEnv(after_set,"");return;}
			*test++ = 0;
			//If the shell is running/exists update the environment
			first_shell->SetEnv(after_set,test);
		}
		delete [] buf2;
	}
}

void AutoexecObject::InstallBefore(const std::string &in) {
	if(GCC_UNLIKELY(installed)) E_Exit("autoexec: already created %s",buf.c_str());
	installed = true;
	buf = in;
	autoexec_strings.push_front(buf);
	this->CreateAutoexec();
}

void AutoexecObject::CreateAutoexec(void) {
	/* Remove old autoexec.bat if the shell exists */
	if(first_shell)	VFILE_Remove("AUTOEXEC.BAT");

	//Create a new autoexec.bat
	autoexec_data[0] = 0;
	size_t auto_len;
	for(auto_it it=  autoexec_strings.begin(); it != autoexec_strings.end(); it++) {
		auto_len = strlen(autoexec_data);
		if ((auto_len+(*it).length()+3)>AUTOEXEC_SIZE) {
			E_Exit("SYSTEM:Autoexec.bat file overflow");
		}
		sprintf((autoexec_data+auto_len),"%s\r\n",(*it).c_str());
	}
	if(first_shell) VFILE_Register("AUTOEXEC.BAT",(Bit8u *)autoexec_data,(Bit32u)strlen(autoexec_data));
}

AutoexecObject::~AutoexecObject(){
	if(!installed) return;

	// Remove the line from the autoexecbuffer and update environment
	for(auto_it it = autoexec_strings.begin(); it != autoexec_strings.end(); ) {
		if((*it) == buf) {
			it = autoexec_strings.erase(it);
			std::string::size_type n = buf.size();
			char* buf2 = new char[n + 1];
			safe_strncpy(buf2, buf.c_str(), n + 1);
			// If it's a environment variable remove it from there as well
			if((strncasecmp(buf2,"set ",4) == 0) && (strlen(buf2) > 4)){
				char* after_set = buf2 + 4;//move to variable that is being set
				char* test = strpbrk(after_set,"=");
				if(!test) continue;
				*test = 0;
				//If the shell is running/exists update the environment
				if(first_shell) first_shell->SetEnv(after_set,"");
			}
			delete [] buf2;
		} else it++;
	}
	this->CreateAutoexec();
}

DOS_Shell::DOS_Shell():Program(){
	input_handle=STDIN;
	echo=true;
	exit=false;
	bf=0;
	call=false;
	completion_start = NULL;
}

Bitu DOS_Shell::GetRedirection(char *s, char **ifn, char **ofn,bool * append) {

	char * lr=s;
	char * lw=s;
	char ch;
	Bitu num=0;
	bool quote = false;
	char* t;

	while ( (ch=*lr++) ) {
		if(quote && ch != '"') { /* don't parse redirection within quotes. Not perfect yet. Escaped quotes will mess the count up */
			*lw++ = ch;
			continue;
		}

		switch (ch) {
		case '"':
			quote = !quote;
			break;
		case '>':
			*append=((*lr)=='>');
			if (*append) lr++;
			lr=ltrim(lr);
			if (*ofn) free(*ofn);
			*ofn=lr;
			while (*lr && *lr!=' ' && *lr!='<' && *lr!='|') lr++;
			//if it ends on a : => remove it.
			if((*ofn != lr) && (lr[-1] == ':')) lr[-1] = 0;
			//			if(*lr && *(lr+1))
			//				*lr++=0;
			//			else
			//				*lr=0;
			t = (char*)malloc(lr-*ofn+1);
			safe_strncpy(t,*ofn,lr-*ofn+1);
			*ofn=t;
			continue;
		case '<':
			if (*ifn) free(*ifn);
			lr=ltrim(lr);
			*ifn=lr;
			while (*lr && *lr!=' ' && *lr!='>' && *lr != '|') lr++;
			if((*ifn != lr) && (lr[-1] == ':')) lr[-1] = 0;
			//			if(*lr && *(lr+1))
			//				*lr++=0;
			//			else
			//				*lr=0;
			t = (char*)malloc(lr-*ifn+1);
			safe_strncpy(t,*ifn,lr-*ifn+1);
			*ifn=t;
			continue;
		case '|':
			ch=0;
			num++;
		}
		*lw++=ch;
	}
	*lw=0;
	return num;
}	

void DOS_Shell::ParseLine(char * line) {
	LOG(LOG_EXEC,LOG_ERROR)("Parsing command line: %s",line);
	/* Check for a leading @ */
	if (line[0] == '@') line[0] = ' ';
	line = trim(line);

	/* Do redirection and pipe checks */

	char * in  = 0;
	char * out = 0;

	Bit16u dummy,dummy2;
	Bit32u bigdummy = 0;
	Bitu num = 0;		/* Number of commands in this line */
	bool append;
	bool normalstdin  = false;	/* wether stdin/out are open on start. */
	bool normalstdout = false;	/* Bug: Assumed is they are "con"      */

	num = GetRedirection(line,&in, &out,&append);
	if (num>1) LOG_MSG("SHELL:Multiple command on 1 line not supported");
	if (in || out) {
		normalstdin  = (psp->GetFileHandle(0) != 0xff); 
		normalstdout = (psp->GetFileHandle(1) != 0xff); 
	}
	if (in) {
		if(DOS_OpenFile(in,OPEN_READ,&dummy)) {	//Test if file exists
			DOS_CloseFile(dummy);
			LOG_MSG("SHELL:Redirect input from %s",in);
			if(normalstdin) DOS_CloseFile(0);	//Close stdin
			DOS_OpenFile(in,OPEN_READ,&dummy);	//Open new stdin
		}
	}
	if (out){
		LOG_MSG("SHELL:Redirect output to %s",out);
		if(normalstdout) DOS_CloseFile(1);
		if(!normalstdin && !in) DOS_OpenFile("con",OPEN_READWRITE,&dummy);
		bool status = true;
		/* Create if not exist. Open if exist. Both in read/write mode */
		if(append) {
			if( (status = DOS_OpenFile(out,OPEN_READWRITE,&dummy)) ) {
				DOS_SeekFile(1,&bigdummy,DOS_SEEK_END);
			} else {
				status = DOS_CreateFile(out,DOS_ATTR_ARCHIVE,&dummy);	//Create if not exists.
			}
		} else {
			status = DOS_OpenFileExtended(out,OPEN_READWRITE,DOS_ATTR_ARCHIVE,0x12,&dummy,&dummy2);
		}

		if(!status && normalstdout) DOS_OpenFile("con",OPEN_READWRITE,&dummy); //Read only file, open con again
		if(!normalstdin && !in) DOS_CloseFile(0);
	}
	/* Run the actual command */
	DoCommand(line);
	/* Restore handles */
	if(in) {
		DOS_CloseFile(0);
		if(normalstdin) DOS_OpenFile("con",OPEN_READWRITE,&dummy);
		free(in);
	}
	if(out) {
		DOS_CloseFile(1);
		if(!normalstdin) DOS_OpenFile("con",OPEN_READWRITE,&dummy);
		if(normalstdout) DOS_OpenFile("con",OPEN_READWRITE,&dummy);
		if(!normalstdin) DOS_CloseFile(0);
		free(out);
	}
}



void DOS_Shell::RunInternal(void)
{
	char input_line[CMD_MAXLINE] = {0};
	while(bf && bf->ReadLine(input_line)) 
	{
		if (echo) {
			if (input_line[0] != '@') {
				ShowPrompt();
				WriteOut_NoParsing(input_line);
				WriteOut_NoParsing("\n");
			};
		};
		ParseLine(input_line);
	}
	return;
}

void DOS_Shell::Run(void) {
	char input_line[CMD_MAXLINE] = {0};
	std::string line;
	if (cmd->FindStringRemainBegin("/C",line)) {
		strcpy(input_line,line.c_str());
		char* sep = strpbrk(input_line,"\r\n"); //GTA installer
		if (sep) *sep = 0;
		DOS_Shell temp;
		temp.echo = echo;
		temp.ParseLine(input_line);		//for *.exe *.com  |*.bat creates the bf needed by runinternal;
		temp.RunInternal();				// exits when no bf is found.
		return;
	}
	/* Start a normal shell and check for a first command init */
	WriteOut("DOSCARD ver.%s bld.%s\n%s\n%s\n\n\n",
			VERSION,BUILDNUMBER,COPYRIGHT_STRING_ORIGINAL,COPYRIGHT_STRING_NEW);

	if (cmd->FindString("/INIT",line,true)) {
		strcpy(input_line,line.c_str());
		line.erase();
		ParseLine(input_line);
	}
	do {
		if (bf){
			if(bf->ReadLine(input_line)) {
				if (echo) {
					if (input_line[0]!='@') {
						ShowPrompt();
						WriteOut_NoParsing(input_line);
						WriteOut_NoParsing("\n");
					};
				};
				ParseLine(input_line);
				if (echo) WriteOut("\n");
			}
		} else {
			if (echo) ShowPrompt();
			InputCommand(input_line);
			ParseLine(input_line);
			if (echo && !bf) WriteOut_NoParsing("\n");
		}
	} while (!exit);
}

void DOS_Shell::SyntaxError(void) {
	WriteOut(SHELL_SYNTAXERROR);
}

class AUTOEXEC {
private:
	AutoexecObject autoexec[17];
	AutoexecObject autoexec_echo;
public:
	AUTOEXEC(void*) {
		//Register a virtual AUOEXEC.BAT file
		VFILE_Register("AUTOEXEC.BAT",(Bit8u *)autoexec_data,(Bit32u)strlen(autoexec_data));
	}
};

static AUTOEXEC* test;

void AUTOEXEC_Init()
{
	test = new AUTOEXEC(NULL);
}

static char const * const path_string="PATH=Z:\\";
static char const * const comspec_string="COMSPEC=Z:\\COMMAND.COM";
static char const * const full_name="Z:\\COMMAND.COM";
static char const * const init_line="/INIT AUTOEXEC.BAT";

void SHELL_Init()
{
	call_shellstop=CALLBACK_Allocate();
	/* Setup the startup CS:IP to kill the last running machine when exitted */
	RealPt newcsip=CALLBACK_RealPointer(call_shellstop);
	SegSet16(cs,RealSeg(newcsip));
	reg_ip=RealOff(newcsip);

	CALLBACK_Setup(call_shellstop,shellstop_handler,CB_IRET,"shell stop");
	PROGRAMS_MakeFile("COMMAND.COM",SHELL_ProgramStart);

	/* Now call up the shell for the first time */
	Bit16u psp_seg=DOS_FIRST_SHELL;
	Bit16u env_seg=DOS_FIRST_SHELL+19; //DOS_GetMemory(1+(4096/16))+1;
	Bit16u stack_seg=DOS_GetMemory(2048/16);
	SegSet16(ss,stack_seg);
	reg_sp=2046;

	/* Set up int 24 and psp (Telarium games) */
	real_writeb(psp_seg+16+1,0,0xea);		/* far jmp */
	real_writed(psp_seg+16+1,1,real_readd(0,0x24*4));
	real_writed(0,0x24*4,((Bit32u)psp_seg<<16) | ((16+1)<<4));

	/* Set up int 23 to "int 20" in the psp. Fixes what.exe */
	real_writed(0,0x23*4,((Bit32u)psp_seg<<16));

	/* Setup MCBs */
	DOS_MCB pspmcb((Bit16u)(psp_seg-1));
	pspmcb.SetPSPSeg(psp_seg);	// MCB of the command shell psp
	pspmcb.SetSize(0x10+2);
	pspmcb.SetType(0x4d);
	DOS_MCB envmcb((Bit16u)(env_seg-1));
	envmcb.SetPSPSeg(psp_seg);	// MCB of the command shell environment
	envmcb.SetSize(DOS_MEM_START-env_seg);
	envmcb.SetType(0x4d);

	/* Setup environment */
	PhysPt env_write=PhysMake(env_seg,0);
	MEM_BlockWrite(env_write,path_string,(Bitu)(strlen(path_string)+1));
	env_write += (PhysPt)(strlen(path_string)+1);
	MEM_BlockWrite(env_write,comspec_string,(Bitu)(strlen(comspec_string)+1));
	env_write += (PhysPt)(strlen(comspec_string)+1);
	mem_writeb(env_write++,0);
	mem_writew(env_write,1);
	env_write+=2;
	MEM_BlockWrite(env_write,full_name,(Bitu)(strlen(full_name)+1));

	DOS_PSP psp(psp_seg);
	psp.MakeNew(0);
	dos.psp(psp_seg);

	/* The start of the filetable in the psp must look like this:
	 * 01 01 01 00 02
	 * In order to achieve this: First open 2 files. Close the first and
	 * duplicate the second (so the entries get 01) */
	Bit16u dummy=0;
	DOS_OpenFile("CON",OPEN_READWRITE,&dummy);	/* STDIN  */
	DOS_OpenFile("CON",OPEN_READWRITE,&dummy);	/* STDOUT */
	DOS_CloseFile(0);							/* Close STDIN */
	DOS_ForceDuplicateEntry(1,0);				/* "new" STDIN */
	DOS_ForceDuplicateEntry(1,2);				/* STDERR */
	DOS_OpenFile("CON",OPEN_READWRITE,&dummy);	/* STDAUX */
	DOS_OpenFile("CON",OPEN_READWRITE,&dummy);	/* STDPRN */

	psp.SetParent(psp_seg);
	/* Set the environment */
	psp.SetEnvironment(env_seg);
	/* Set the command line for the shell start up */
	CommandTail tail;
	tail.count=(Bit8u)strlen(init_line);
	strcpy(tail.buffer,init_line);
	MEM_BlockWrite(PhysMake(psp_seg,128),&tail,128);

	/* Setup internal DOS Variables */
	dos.dta(RealMake(psp_seg,0x80));
	dos.psp(psp_seg);


	SHELL_ProgramStart(&first_shell);
	first_shell->Run();
	delete first_shell;
	first_shell = 0;//Make clear that it shouldn't be used anymore
}

}
