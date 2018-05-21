// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include "VM86.h"
#include <time.h>
#include <sys/timeb.h>

#ifndef USE_RAW_OUTPUT
#include <stdio.h>
#include <ctype.h>
#endif

#include <unistd.h>
#ifndef USRIO
#include <fcntl.h>
#else
//typedef unsigned long long size_t;
/*Include floppy disk image data*/
#include "floppyimg.h"
#endif

#ifdef USRIO
static time_t seconds = 1234789;
static unsigned short millis = 0;
static size_t diskhead = 0;
#endif

#ifdef MRAM_TEST
void my_memcpy(RAMptr<uch>* p, const void* src, unsigned len)
{
	unsigned i;
	const uch* ptr = (const uch*)src;
	for (i = 0; i < len; i++)
		(*p)[i] = ptr[i];
	printf("memcpy(): %u bytes copied\n",i);
}

ssize_t my_read(int id, RAMptr<uch>* p, size_t n)
{
	unsigned i;
	uch tmp;
	for (i = 0; i < n; ) {
		if (read(id,&tmp,1) == 1) (*p)[i++] = tmp;
		else break;
	}
	return i;
}

ssize_t my_write(int id, RAMptr<uch>* p, size_t n)
{
	unsigned i;
	for (i = 0; i < n; ) {
		if (write(id,&((*p)[i++]),1) != 1) break;
	}
	return i;
}
#endif /*MRAM_TEST*/

#ifdef USRIO
size_t sta_read(int id, void* buf, size_t n)
{
	if (diskhead + n >= fd_img_len) return 0;
	memcpy(buf,fd_img+diskhead,n);
	return n;
}

size_t sta_write(int id, const void* buf, size_t n)
{
	if (diskhead + n >= fd_img_len) return 0;
	memcpy(fd_img+diskhead,buf,n);
	return n;
}

void memcpy(void* dst, const void* src, unsigned len)
{
	char* to = (char*)dst;
	const char* from = (const char*)src;
	while (len > 0) {
		--len;
		to[len] = from[len];
	}
}
#endif /*USRIO*/

void VM86::LocalOpcode()
{
#ifdef MRAM_TEST
	RAMptr<uch> tmp(&mem,SEGREG(REG_ES, REG_BX,));
#else
	unsigned char* tmp = mem + SEGREG(REG_ES, REG_BX,);
#endif

#ifndef USRIO
	time_t clock_buf;
	struct timeb ms_clock;
#endif

	switch ((char)i_data0)
	{
		OPCODE_CHAIN 0: // PUTCHAR_AL
#ifndef USE_RAW_OUTPUT
			if ((*regs8 == 10 || *regs8 == 13) || isprint(*regs8))
				putchar(*regs8);
			else
				printf("byte out: %hhu\n",*regs8);
#else
//#ifdef USRIO
//			//TODO
//#else
			write(1, regs8, 1);
//#endif /*USRIO*/
#endif /* ! USE_RAW_OUTPUT */

		OPCODE 1: // GET_RTC
#ifndef USRIO
			time(&clock_buf);
			ftime(&ms_clock);
#ifdef MRAM_TEST
			my_memcpy(&tmp, localtime(&clock_buf), sizeof(struct tm));
#else
			memcpy(tmp, localtime(&clock_buf), sizeof(struct tm));
#endif /*MRAM_TEST*/
			CAST(short)mem[SEGREG(REG_ES, REG_BX, 36+)] = ms_clock.millitm;
#else
			if (++millis >= 1000) {
				millis = 0;
				seconds++;
			} else
				millis += TIMESTEP;
			memcpy(tmp, localtime(&seconds), sizeof(seconds));
			CAST(short)mem[SEGREG(REG_ES, REG_BX, 36+)] = millis;
#endif /* ! USRIO */

		OPCODE 2: // DISK_READ
#ifndef USRIO
			regs8[REG_AL] = ~lseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, 0)
#ifdef MRAM_TEST
				? (int)my_read(disk[regs8[REG_DL]], &tmp, regs16[REG_AX])
#else
				? (int)read(disk[regs8[REG_DL]], tmp, regs16[REG_AX])
#endif /*MRAM_TEST*/
				: 0;
#else
			if ((disk[regs8[REG_DL]]) && ((CAST(unsigned)regs16[REG_BP] << 9) < fd_img_len)) {
				diskhead = (CAST(unsigned)regs16[REG_BP] << 9);
				regs8[REG_AL] = sta_read(disk[regs8[REG_DL]], tmp, regs16[REG_AX]);
			} else
				regs8[REG_AL] = 0;
#endif /* ! USRIO*/

		OPCODE 3: // DISK_WRITE
#ifndef USRIO
			regs8[REG_AL] = ~lseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, 0)
#if MRAM_TEST
				? (int)my_write(disk[regs8[REG_DL]], &tmp, regs16[REG_AX])
#else
				? (int)write(disk[regs8[REG_DL]], tmp, regs16[REG_AX])
#endif /*MRAM_TEST*/
				: 0;
#else
			if ((disk[regs8[REG_DL]]) && ((CAST(unsigned)regs16[REG_BP] << 9) < fd_img_len)) {
				diskhead = (CAST(unsigned)regs16[REG_BP] << 9);
				regs8[REG_AL] = sta_write(disk[regs8[REG_DL]], tmp, regs16[REG_AX]);
			} else
				regs8[REG_AL] = 0;
#endif /* ! USRIO */
	}
}

void VM86::OpenDD()
{
#ifndef USRIO
	// Open floppy disk image (disk[1]), and hard disk image (disk[0]) if specified
	disk[1] = open("fd.raw", 32898);
	disk[0] = 0;

	// Set CX:AX equal to the hard disk image size, if present
#ifndef MRAM_TEST
	CAST(unsigned)regs16[REG_AX] = *disk ? lseek(*disk, 0, 2) >> 9 : 0;
#else
	regs16[REG_AX] = 0; //no hdd
#endif

#else

	//We'll use static data array in memory
	disk[1] = 1;
	disk[0] = 0;
	regs16[REG_AX] = 0; //no hdd
#endif /*USRIO*/
}

void VM86::CloseDD()
{
#ifndef USRIO
	for (int i = 0; i < NUMVDISKS; i++) {
		if (disk[i]) close(disk[i]);
		disk[i] = 0;
	}
#endif
}
