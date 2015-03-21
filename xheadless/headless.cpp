/*
 *  Copyright (C) 2013-2014  Dmitry Soloviov
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
#include <vector>
//#include <fcntl.h>
#include "doscard.h"

#define BITFILE_ALTPATH "../libdoscard/libdbwrap.bc"
#define INSTR_BUFLEN 64

using namespace doscard;

int main(int argc, char* argv[])
{
	CDosCard* vm;				//virtual machine
	char istr[INSTR_BUFLEN];	//input string
	int ml;						//marker prefix length (const)
	char xc;					//current character (x)
	bool cr,nl;					//carriage return / line feed presence flags
	LDBI_MesgVec::iterator i;	//vector iterator
	unsigned int j;				//simple iterator :)
	LDBI_MesgVec* msg;			//external (VM's) messages vector ptr
	bool nomsg;					//do not show messages flag
	dosbox::LDB_Settings* vmset;//VM settings

	printf("\n---------------------------------------------------\n");
	printf("|              DosCard Headless Mode              |\n");
	printf("| (C) Dmitry 'MatrixS_Master' Soloviov, 2013-2014 |\n");
	printf("---------------------------------------------------\n\n");

	nomsg = false;
	if (argc > 1) {
		if (!strcmp(argv[1],"-help")) {
			printf("Usage: %s [option]\n",argv[0]);
			printf("Possible options are:\n-help\n-nomsg\n");
			return 0;
		} else if (!strcmp(argv[1],"-nomsg"))
			nomsg = true;
	}

	//prepare everything
	ml = strlen(DOSCRD_EHOUT_MARKER);
	memset(istr,0,sizeof(istr));
	//fcntl(0,F_SETFL,fcntl(0,F_GETFL) | O_NONBLOCK); //for nonblocking input
	//init libdoscard
	if (nomsg) LibDosCardInit(0); //no debugging
	else LibDosCardInit(1); //with some debug output

	//to create a shiny new VM
	vm = new CDosCard(false);
	if (!vm->TryLoad(BITFILE_ALTPATH)) {
		printf("Couldn't load VM!\n");
		abort();
	}

	//and prepare it
	if (!vm->Prepare()) {
		printf("Couldn't prepare VM for execution!\n");
		abort();
	}
	vm->SetCapabilities(DOSCRD_CAPS_HEADLESS); //headless mode set
	vmset = vm->GetSettings();
	if (!vmset) {
		printf("Unable to get VM settings!\n");
		abort();
	}
	vmset->mem.total_ram = 4; //set to 4MB of phys RAM
	vm->ApplySettings(vmset);

	//and Run it!
	vm->Run();

	//Main Loop
	printf("Loop started\n");
	while (vm->GetCurrentState() == DOSCRD_RUNNING) {
		//process output
		msg = vm->GetMessages();
		if ((msg) && (!msg->empty())) {

			cr = false;
			nl = true;
			for (i = msg->begin(); i != msg->end(); i++) {
				if (i->find(DOSCRD_EHOUT_MARKER)) {
					//marker not found at the beginning of string
					if (!nomsg) {
						//well, we can show the message for ya
						if (!nl) putchar('\n'); //we want it at new line
						printf("[MSG]: %s\n",i->c_str());
						nl = true;
					}
					continue;
				}

				//marker found, process VM's STDIN
				for (j = ml; j < i->length(); j++) {
					xc = i->c_str()[j];
					if (xc == '\r') cr = true;	//shall remember CR
					else {
						nl = (xc == '\n');
						if ((isprint(xc)) || nl || cr) {
							putchar(xc);		//if xc==\n we're happy too :)
						} else
							printf("[0x%02X]",xc); //just print out a hex code
						cr = false; //doesn't matter if there was LF
					}
				}
			}
			msg->clear();
		}

		//process input
		if (read(0,istr,sizeof(istr)-1) > 0) {
			vm->PutString(istr);
			memset(istr,0,sizeof(istr)); //cleanup
		}
	}
	printf("Loop ended\n");

	//Free mem
	printf("Deleting machine...\n");
	delete vm;
	LibDosCardExit();

	//and quit without executing all atexit() registered functions
	printf("Quit\n");
	_exit(EXIT_SUCCESS);
}
