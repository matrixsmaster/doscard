// ldctest3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include "doscard_common.h"
#include "doscard_vs.h"
#include "doscard_ldbvs.h"

#define INSTR_BUFLEN 64
#define WAITME puts("Press Enter..."); getchar()

using namespace dosbox;
using namespace doscard;

int _tmain(int argc, _TCHAR* argv[])
{
	char istr[INSTR_BUFLEN];	//input string
	int ml;						//marker prefix length (const)
	char xc;					//current character (x)
	bool cr,nl;					//carriage return / line feed presence flags
	LDBI_MesgVec::iterator i;	//vector iterator
	unsigned int j;				//simple iterator :)
	LDBI_MesgVec* msg;			//external (VM's) messages vector ptr
	bool nomsg;					//do not show messages flag
	LDB_Settings* vmset;//VM settings

	puts("Hello from main()\n");
	WinDosCardInit(1);
	WAITME;

	if (WinDosCardPrepare()) puts("Prepared!");
	WAITME;

	printf("Caps setting: %d\n",WinDosCardSetCapabilities(DOSCRD_CAPS_HEADLESS));
	WAITME;

	vmset = WinDosCardGetSettings();
	if (!vmset) {
		printf("Unable to get VM settings!\n");
		abort();
	}
	puts("Got VM settings!");
	vmset->mem.total_ram = 4; //set to 4MB of phys RAM
	if (WinDosCardApplySettings(vmset))
		puts("Settings applied!");
	WAITME;

	puts("Running!");
	printf("Run result = %d\n",WinDosCardRun());
	WAITME;

	WinDosCardExit();
	WAITME;

	return 0;
}

