/*
 *  Copyright (C) 2013-2015  Dmitry Soloviov
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

#include "doscard.h"

#ifndef BITFILE_ALTPATH
#define BITFILE_ALTPATH "./libdbwrap.bc"
#endif

using namespace doscard;

int main(int argc, char* argv[])
{
	CDosCard* vm;				//VM
	dosbox::LDB_Settings* vmset;//VM settings

	printf("\nDosCard XTest\n");

	LibDosCardInit(1);

	for (;;) {
		printf("\n\n******* NEW VM *******\n");

		//Create a shiny new VM
		vm = new CDosCard(false);
		if (!vm->TryLoad(BITFILE_ALTPATH)) {
			printf("Couldn't load VM!\n");
			abort();
		}
		printf("Version info:\n%s\n",vm->GetVersionStringSafe());

		//and prepare it
		if (!vm->Prepare()) {
			printf("Couldn't prepare VM for execution!\n");
			abort();
		}
		vm->SetCapabilities(DOSCRD_CAPS_BASIC);// | DOSCRD_CAP_EVENT);

		//Change virtual RAM size
		vmset = vm->GetSettings();
		if (!vmset) {
			printf("Unable to get VM settings!\n");
			abort();
		}
		vmset->mem.total_ram = 4; //set to 4MB of phys RAM
		vm->ApplySettings(vmset);

		vm->Run();

		getchar();

		//Free mem
		printf("Deleting machine...\n");
		delete vm;

		getchar();
	}

	LibDosCardExit();

	//Quit without executing all atexit() registered functions
	printf("Quit\n");
	_exit(EXIT_SUCCESS);
}
