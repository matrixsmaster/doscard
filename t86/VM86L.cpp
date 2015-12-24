// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include <unistd.h>
#include <fcntl.h>
#include "VM86.h"

void VM86::LocalOpcode()
{
	switch ((char)i_data0)
	{
		OPCODE_CHAIN 0: // PUTCHAR_AL
			write(1, regs8, 1)

		OPCODE 1: // GET_RTC
			time(&clock_buf);
			ftime(&ms_clock);
			memcpy(mem + SEGREG(REG_ES, REG_BX,), localtime(&clock_buf), sizeof(struct tm));
			CAST(short)mem[SEGREG(REG_ES, REG_BX, 36+)] = ms_clock.millitm;

		OPCODE 2: // DISK_READ
			regs8[REG_AL] = ~lseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, 0)
				? (int)read(disk[regs8[REG_DL]], mem + SEGREG(REG_ES, REG_BX,), regs16[REG_AX])
				: 0;

		OPCODE 3: // DISK_WRITE
			regs8[REG_AL] = ~lseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, 0)
				? (int)write(disk[regs8[REG_DL]], mem + SEGREG(REG_ES, REG_BX,), regs16[REG_AX])
				: 0;
	}
}
