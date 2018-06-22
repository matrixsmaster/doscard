// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include <time.h>
//#include <sys/timeb.h>
#include <ctype.h>
#include <unistd.h>
//#include <fcntl.h>
//#include <memory.h>
#include "VM86.h"
#include "VM86macro.h"
#include "mthreads.h"

/* Locals */
static time_t seconds = 1234789;
static uint16_t millis = 0;
static size_t diskhead = 0;

static uint8_t const fd_img[] = {
0,0,0
};

/*Static read/write IO*/
size_t sta_read(int, void* buf, size_t n)
{
	if (diskhead + n >= sizeof(fd_img)) return 0;
	memcpy(buf,fd_img+diskhead,n);
	return n;
}

size_t sta_write(int, const void* buf, size_t n)
{
	if (diskhead + n >= sizeof(fd_img)) return 0;
	//memcpy(fd_img+diskhead,buf,n);
	return n;
}

void VM86::LocalOpcode()
{
	uint8_t* tmp = mem + SEGREG(REG_ES, REG_BX,);

	switch ((char)i_data0)
	{
		OPCODE_CHAIN 0: // PUTCHAR_AL
//			if ((*regs8 == 10 || *regs8 == 13) || isprint(*regs8))
				PushOutput(*regs8);

		OPCODE 1: // GET_RTC
			if (++millis >= 1000) {
				millis = 0;
				seconds++;
			} else
				millis += TIMESTEP;
			memcpy(tmp, localtime(&seconds), sizeof(seconds));
			CAST(int16_t,mem[SEGREG(REG_ES, REG_BX, 36+)]) = millis;

		OPCODE 2: // DISK_READ
			if ((disk[regs8[REG_DL]]) && ((CAST(unsigned,regs16[REG_BP]) << 9) < sizeof(fd_img))) {
				diskhead = (CAST(unsigned,regs16[REG_BP]) << 9);
				regs8[REG_AL] = sta_read(disk[regs8[REG_DL]], tmp, regs16[REG_AX]);
			} else
				regs8[REG_AL] = 0;

		OPCODE 3: // DISK_WRITE
			if ((disk[regs8[REG_DL]]) && ((CAST(unsigned,regs16[REG_BP]) << 9) < sizeof(fd_img))) {
				diskhead = (CAST(unsigned,regs16[REG_BP]) << 9);
				regs8[REG_AL] = sta_write(disk[regs8[REG_DL]], tmp, regs16[REG_AX]);
			} else
				regs8[REG_AL] = 0;
	}
}

void VM86::OpenDD()
{
	//We'll use static data array in memory
	disk[1] = 1;
	disk[0] = 0;

	// Set CX:AX equal to the hard disk image size, if present
	//CAST(unsigned)regs16[REG_AX] = *disk ? lseek(*disk, 0, 2) >> 9 : 0;
	regs16[REG_AX] = 0; //no hdd
}

void VM86::CloseDD()
{
	//Nothing to do
}

void VM86::PushOutput(int ch)
{
	minmutex_lock(&io_lock);
	output_chars.push_back(ch);
	minmutex_unlock(&io_lock);
}

bool VM86::PullInput(int &ch)
{
	bool r;
	minmutex_lock(&io_lock);
	r = !input_chars.empty();
	if (r) {
		ch = input_chars.front();
		input_chars.pop_front();
	}
	minmutex_unlock(&io_lock);
	return r;
}
