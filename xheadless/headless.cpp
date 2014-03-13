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
	printf("Loop started\n");
	while (vm->GetCurrentState() == DOSCRD_RUNNING) {
		LDBI_MesgVec* msg = vm->GetMessages();
		if ((!msg) || (msg->empty())) continue;
		LDBI_MesgVec::iterator i;
		bool cr = false;
		for (i = msg->begin(); i != msg->end(); i++) {
			if (i->find(DOSCRD_EHOUT_MARKER)) {
				printf("[MSG]: %s\n",i->c_str());
				continue;
			}
//			printf("%s",i->c_str()+strlen(DOSCRD_EHOUT_MARKER));
			for (int j=ml; j<i->length(); j++) {
				char xc = i->c_str()[j];
				if (xc == '\r') cr = true;
				else {
					if ((isprint(xc)) || ((xc == '\n') && (cr)))
						putchar('\n');
					else
						printf("[0x%02X]",xc);
					cr = false;
				}
			}
		}
		msg->clear();
	}
	printf("Loop ended\n");
	printf("Deleting machine...\n");
	delete vm;
	LibDosCardExit();
	printf("Quit\n");
	_exit(EXIT_SUCCESS);
}
