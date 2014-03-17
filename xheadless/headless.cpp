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

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "doscard.h"
#include "../xshell/xskbd.h"

#define BITFILE_ALTPATH "../libdoscard/libdbwrap.bc"

using namespace doscard;

int main(int, char**)
{
	printf("Alive\n");
	LibDosCardInit(1);
	CDosCard* vm = new CDosCard(false);
	if (!vm->TryLoad(BITFILE_ALTPATH)) {
		printf("Couldn't load VM!\n");
		abort();
	}
	if (!vm->Prepare()) abort();
	vm->SetCapabilities(DOSCRD_CAPS_HEADLESS);
	vm->Run();
	int ml = strlen(DOSCRD_EHOUT_MARKER);
	char* istr = reinterpret_cast<char*> (malloc(1024));
	char xc;
	printf("Loop started\n");
	fcntl(0,F_SETFL,fcntl(0,F_GETFL) | O_NONBLOCK);
	while (vm->GetCurrentState() == DOSCRD_RUNNING) {
		LDBI_MesgVec* msg = vm->GetMessages();
		if ((msg) && (!msg->empty())) {
			LDBI_MesgVec::iterator i;
			bool cr = false;
			bool nl = true;

			//process output
			for (i = msg->begin(); i != msg->end(); i++) {
				if (i->find(DOSCRD_EHOUT_MARKER)) {
					if (!nl) putchar('\n');
					printf("[MSG]: %s\n",i->c_str());
					nl = true;
					continue;
				}
				for (int j=ml; j<i->length(); j++) {
					xc = i->c_str()[j];
					if (xc == '\r') cr = true;
					else {
						nl = (xc == '\n');
						if ((isprint(xc)) || (nl && cr)) {
							putchar(xc);
						} else
							printf("[0x%02X]",xc);
						cr = false;
					}
				}
			}
			msg->clear();
		}

		//process input
		memset(istr,0,1024); //FIXME
		if (read(0,istr,1023) > 0)
			vm->PutString(istr);
	}
	free(istr);
	printf("Loop ended\n");
	printf("Deleting machine...\n");
	delete vm;
	LibDosCardExit();
	printf("Quit\n");
	_exit(EXIT_SUCCESS);
}
