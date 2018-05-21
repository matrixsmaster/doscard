// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include "VM86.h"
#include "VM86bios.h"
#include <unistd.h>

VM86::VM86()
{
	memcpy(cga_colors,cga_colors_table,sizeof(cga_colors));
//	memset(disk,0,sizeof(disk));
	for (int i = 0; i < NUMVDISKS; i++) disk[i] = 0;
	pause = 0;

#ifdef MRAM_TEST
	io_ports.Segment(RAM_SIZE+1);
	opcode_stream.Ram(&mem);
#endif

	Reset();
}

VM86::~VM86()
{
	CloseDD();
}

void VM86::Reset()
{
	CloseDD();

	// regs16 and reg8 point to F000:0, the start of memory-mapped registers. CS is initialised to F000
#ifndef MRAM_TEST
	regs16 = (unsigned short *)(regs8 = mem + REGS_BASE);
#else
	regs8.Reset();
	regs8.Ram(&mem);
	regs8 += REGS_BASE;
	regs16.Reset();
	regs16.Ram(&mem_us);
	regs16 += REGS_BASE/2;
#endif
	regs16[REG_CS] = 0xF000;

	// Trap flag off
	regs8[FLAG_TF] = 0;

	// Set DL equal to the boot device: 0 for the FD, or 0x80 for the HD. Normally, boot from the FD.
	// But, if the HD image file is prefixed with @, then boot from the HD
	regs8[REG_DL] = 0;

	OpenDD();

	// Load BIOS image into F000:0100, and set IP to 0100
	reg_ip = 0x100;
#ifdef MRAM_TEST
	RAMptr<uch> tmp = regs8 + reg_ip;
	my_memcpy(&tmp, bios, bios_len);
#else
	memcpy(regs8 + reg_ip, bios, bios_len);
#endif
}

// Set carry flag
char VM86::set_CF(int new_CF)
{
	return regs8[FLAG_CF] = !!new_CF;
}

// Set auxiliary flag
char VM86::set_AF(int new_AF)
{
	return regs8[FLAG_AF] = !!new_AF;
}

// Set overflow flag
char VM86::set_OF(int new_OF)
{
	return regs8[FLAG_OF] = !!new_OF;
}

// Set auxiliary and overflow flag after arithmetic operations
char VM86::set_AF_OF_arith()
{
	set_AF((op_source ^= op_dest ^ op_result) & 0x10);
	if (op_result == op_dest)
		return set_OF(0);
	else
		return set_OF(1 & (regs8[FLAG_CF] ^ op_source >> (TOP_BIT - 1)));
}

// Assemble and return emulated CPU FLAGS register in scratch_uint
void VM86::make_flags()
{
	scratch_uint = 0xF002; // 8086 has reserved and unused flags set to 1
	for (int i = 9; i--;)
		scratch_uint += regs8[FLAG_CF + i] << lookup_table[TABLE_FLAGS_BITFIELDS][i];
}

// Set emulated CPU FLAGS register from regs8[FLAG_xx] values
void VM86::set_flags(int new_flags)
{
	for (int i = 9; i--;)
		regs8[FLAG_CF + i] = !!(1 << lookup_table[TABLE_FLAGS_BITFIELDS][i] & new_flags);
}

// Convert raw opcode to translated opcode index. This condenses a large number of different encodings of similar
// instructions into a much smaller number of distinct functions, which we then execute
void VM86::set_opcode(unsigned char opcode)
{
	xlat_opcode_id = lookup_table[TABLE_XLAT_OPCODE][raw_opcode_id = opcode];
	extra = lookup_table[TABLE_XLAT_SUBFUNCTION][opcode];
	i_mod_size = lookup_table[TABLE_I_MOD_SIZE][opcode];
	set_flags_type = lookup_table[TABLE_STD_FLAGS][opcode];
}

// Execute INT #interrupt_num on the emulated machine
char VM86::pc_interrupt(unsigned char interrupt_num)
{
	set_opcode(0xCD); // Decode like INT

	make_flags();
	R_M_PUSH(scratch_uint);
	R_M_PUSH(regs16[REG_CS]);
	R_M_PUSH(reg_ip);
	MEM_OP(REGS_BASE + 2 * REG_CS, =, 4 * interrupt_num + 2);
	R_M_OP(reg_ip, =, mem[4 * interrupt_num]);

	return regs8[FLAG_TF] = regs8[FLAG_IF] = 0;
}

// AAA and AAS instructions - which_operation is +1 for AAA, and -1 for AAS
int VM86::AAA_AAS(char which_operation)
{
	return (regs16[REG_AX] += 262 * which_operation*set_AF(set_CF(((regs8[REG_AL] & 0x0F) > 9) || regs8[FLAG_AF])), regs8[REG_AL] &= 0x0F);
}

// Decode mod, r_m and reg fields in instruction
void VM86::DecodeRM_REG()
{
	scratch2_uint = 4 * !i_mod;
	op_to_addr = rm_addr = i_mod < 3 ?
			SEGREG(seg_override_en ? seg_override : lookup_table[scratch2_uint + 3][i_rm], lookup_table[scratch2_uint][i_rm], regs16[lookup_table[scratch2_uint + 1][i_rm]] + lookup_table[scratch2_uint + 2][i_rm] * i_data1+)
			: GET_REG_ADDR(i_rm);
	op_from_addr = GET_REG_ADDR(i_reg);
	if (i_d) {
		scratch_uint = op_from_addr;
		op_from_addr = rm_addr;
		op_to_addr = scratch_uint;
	}
}
//#include <stdio.h>
// [I]MUL/[I]DIV/DAA/DAS/ADC/SBB helpers
void VM86::MUL()
{
//	printf("MUL\n");
	set_opcode(0x10);
	if (i_w) {
		op_result = CAST(unsigned short)mem[rm_addr] * (unsigned short)*regs16;
	} else {
		op_result = CAST(unsigned char)mem[rm_addr] * (unsigned char)*regs8;
	}
	if (i_w)
		regs16[i_w + 1] = op_result >> 16;
	else
		regs8[i_w + 1] = op_result >> 16;
	regs16[REG_AX] = op_result;
	if (i_w)
		set_OF(set_CF(op_result - (unsigned short)op_result));
	else
		set_OF(set_CF(op_result - (unsigned char)op_result));
}

void VM86::IMUL()
{
//	printf("IMUL\n");
	set_opcode(0x10);
	if (i_w) {
		op_result = CAST(short)mem[rm_addr] * (short)*regs16;
	} else {
		op_result = CAST(char)mem[rm_addr] * (char)*regs8;
	}
	if (i_w)
		regs16[i_w + 1] = op_result >> 16;
	else
		regs8[i_w + 1] = op_result >> 16;
	regs16[REG_AX] = op_result;
	if (i_w)
		set_OF(set_CF(op_result - (short)op_result));
	else
		set_OF(set_CF(op_result - (char)op_result));
}

void VM86::DIV()
{
//	printf("DIV\n");
	if (i_w) {
		scratch_int = CAST(unsigned short)mem[rm_addr];
		if (!scratch_int) { pc_interrupt(0); return; }
		scratch2_uint = (unsigned)(scratch_uint = (regs16[i_w+1] << 16) + regs16[REG_AX]) / scratch_int;
		if (scratch2_uint - (unsigned short)scratch2_uint) { pc_interrupt(0); return; }
		regs16[i_w+1] = scratch_uint - scratch_int * (*regs16 = scratch2_uint);
	} else {
		scratch_int = CAST(unsigned char)mem[rm_addr];
		if (!scratch_int) { pc_interrupt(0); return; }
		scratch2_uint = (unsigned short)(scratch_uint = (regs8[i_w+1] << 16) + regs16[REG_AX]) / scratch_int;
		if (scratch2_uint - (unsigned char)scratch2_uint) { pc_interrupt(0); return; }
		regs8[i_w+1] = scratch_uint - scratch_int * (*regs8 = scratch2_uint);
	}
}

void VM86::IDIV()
{
//	printf("IDIV\n");
	if (i_w) {
		scratch_int = CAST(short)mem[rm_addr];
		if (!scratch_int) { pc_interrupt(0); return; }
		scratch2_uint = (int)(scratch_uint = (regs16[i_w+1] << 16) + regs16[REG_AX]) / scratch_int;
		if (scratch2_uint - (short)scratch2_uint) { pc_interrupt(0); return; }
		regs16[i_w+1] = scratch_uint - scratch_int * (*regs16 = scratch2_uint);
	} else {
		scratch_int = CAST(char)mem[rm_addr];
		if (!scratch_int) { pc_interrupt(0); return; }
		scratch2_uint = (short)(scratch_uint = (regs8[i_w+1] << 16) + regs16[REG_AX]) / scratch_int;
		if (scratch2_uint - (char)scratch2_uint) { pc_interrupt(0); return; }
		regs8[i_w+1] = scratch_uint - scratch_int * (*regs8 = scratch2_uint);
	}
}

/*
 * Decimal adjust After Addition.
Corrects the result of addition of two packed BCD values.

Algorithm:

if low nibble of AL > 9 or AF = 1 then:
	AL = AL + 6
	AF = 1
if AL > 9Fh or CF = 1 then:
	AL = AL + 60h
	CF = 1
	**********************
CF_old = CF
IF (al AND 0Fh > 9) or (the Auxilliary Flag is set)
   al = al+6
   CF = CF or CF_old
   AF set
ENDIF
IF (al > 99h) or (Carry Flag is set)
   al = al + 60h
   CF set
ENDIF
 */
void VM86::DAA()
{
//	printf("DAA\n");
	if (set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF])) {
		op_result = regs8[REG_AL] += 6;
		set_CF(regs8[FLAG_CF] || (regs8[REG_AL] < scratch2_uint));
	}
	set_CF((regs8[REG_AL] > 0x99) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] += 0x60);
}

/*
 * Decimal adjust After Subtraction.
Corrects the result of subtraction of two packed BCD values.

Algorithm:

if low nibble of AL > 9 or AF = 1 then:
	AL = AL - 6
	AF = 1
if AL > 9Fh or CF = 1 then:
	AL = AL - 60h
	CF = 1
	*****************
   CF_old = CF
IF (al AND 0Fh > 9) or (the Auxilliary Flag is set)
   al = al-6
   CF = CF or CF_old
   AF set
ENDIF
IF (al > 99h) or (Carry Flag is set)
   al = al - 60h
   CF set
ENDIF
 */
void VM86::DAS()
{
//	printf("DAS\n");
	if (set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF])) {
		op_result = regs8[REG_AL] -= 6;
		set_CF(regs8[FLAG_CF] || (regs8[REG_AL] >= scratch2_uint));
	}
	set_CF((regs8[REG_AL] > 0x99) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] -= 0x60);
}

/*
void VM86::audio_callback(void *data, unsigned char *stream, int len)
{
	for (int i = 0; i < len; i++)
		stream[i] = (spkr_en == 3) && CAST(unsigned short)mem[0x4AA] ? -((54 * wave_counter++ / CAST(unsigned short)mem[0x4AA]) & 1) : sdl_audio.silence;

	spkr_en = io_ports[0x61] & 3;
}
*/

void VM86::Step()
{
	if (pause) return;

	// Set up variables to prepare for decoding an opcode
	set_opcode(*opcode_stream);

	// Extract i_w and i_d fields from instruction
	i_w = (i_reg4bit = raw_opcode_id & 7) & 1;
	i_d = i_reg4bit / 2 & 1;

	// Extract instruction data fields
	i_data0 = CAST(short)opcode_stream[1];
	i_data1 = CAST(short)opcode_stream[2];
	i_data2 = CAST(short)opcode_stream[3];

	// seg_override_en and rep_override_en contain number of instructions to hold segment override and REP prefix respectively
	if (seg_override_en)
		seg_override_en--;
	if (rep_override_en)
		rep_override_en--;

	// i_mod_size > 0 indicates that opcode uses i_mod/i_rm/i_reg, so decode them
	if (i_mod_size)
	{
		i_mod = (i_data0 & 0xFF) >> 6;
		i_rm = i_data0 & 7;
		i_reg = i_data0 / 8 & 7;

		if ((!i_mod && i_rm == 6) || (i_mod == 2))
			i_data2 = CAST(short)opcode_stream[4];
		else if (i_mod != 1)
			i_data2 = i_data1;
		else // If i_mod is 1, operand is (usually) 8 bits rather than 16 bits
			i_data1 = (char)i_data1;

		DecodeRM_REG();
	}

	// Execute
	IEU();

	// Increment instruction pointer by computed instruction length. Tables in the BIOS binary
	// help us here.
	reg_ip += (i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6))*i_mod_size + lookup_table[TABLE_BASE_INST_SIZE][raw_opcode_id] + lookup_table[TABLE_I_W_SIZE][raw_opcode_id]*(i_w + 1);

	// If instruction needs to update SF, ZF and PF, set them as appropriate
	if (set_flags_type & FLAGS_UPDATE_SZP)
	{
		regs8[FLAG_SF] = SIGN_OF(op_result);
		regs8[FLAG_ZF] = !op_result;
		regs8[FLAG_PF] = lookup_table[TABLE_PARITY_FLAG][(unsigned char)op_result];

		// If instruction is an arithmetic or logic operation, also set AF/OF/CF as appropriate.
		if (set_flags_type & FLAGS_UPDATE_AO_ARITH)
			set_AF_OF_arith();
		if (set_flags_type & FLAGS_UPDATE_OC_LOGIC)
			set_CF(0), set_OF(0);
	}

	// Poll timer/keyboard every KEYBOARD_TIMER_UPDATE_DELAY instructions
	if (!(++inst_counter % KEYBOARD_TIMER_UPDATE_DELAY))
		int8_asap = 1;

#if 0
	// Update the video graphics display every GRAPHICS_UPDATE_DELAY instructions
	if (!(inst_counter % GRAPHICS_UPDATE_DELAY))
	{
		// Video card in graphics mode?
		if (io_ports[0x3B8] & 2)
		{/*
			// If we don't already have an SDL window open, set it up and compute color and video memory translation tables
			if (!sdl_screen)
			{
				for (int i = 0; i < 16; i++)
					pixel_colors[i] = mem[0x4AC] ? // CGA?
						cga_colors[(i & 12) >> 2] + (cga_colors[i & 3] << 16) // CGA -> RGB332
						: 0xFF*(((i & 1) << 24) + ((i & 2) << 15) + ((i & 4) << 6) + ((i & 8) >> 3)); // Hercules -> RGB332

				for (int i = 0; i < GRAPHICS_X * GRAPHICS_Y / 4; i++)
					vid_addr_lookup[i] = i / GRAPHICS_X * (GRAPHICS_X / 8) + (i / 2) % (GRAPHICS_X / 8) + 0x2000*(mem[0x4AC] ? (2 * i / GRAPHICS_X) % 2 : (4 * i / GRAPHICS_X) % 4);

				SDL_Init(SDL_INIT_VIDEO);
				sdl_screen = SDL_SetVideoMode(GRAPHICS_X, GRAPHICS_Y, 8, 0);
				SDL_EnableUNICODE(1);
				SDL_EnableKeyRepeat(500, 30);
			}

			// Refresh SDL display from emulated graphics card video RAM
			vid_mem_base = mem + 0xB0000 + 0x8000*(mem[0x4AC] ? 1 : io_ports[0x3B8] >> 7); // B800:0 for CGA/Hercules bank 2, B000:0 for Hercules bank 1
			for (int i = 0; i < GRAPHICS_X * GRAPHICS_Y / 4; i++)
				((unsigned *)sdl_screen->pixels)[i] = pixel_colors[15 & (vid_mem_base[vid_addr_lookup[i]] >> 4*!(i & 1))];

			SDL_Flip(sdl_screen);*/
		}/*
		else if (sdl_screen) // Application has gone back to text mode, so close the SDL window
		{
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
			sdl_screen = 0;
		}
		SDL_PumpEvents();*/
	}
#endif

	// Application has set trap flag, so fire INT 1
	if (trap_flag)
		pc_interrupt(1);

	trap_flag = regs8[FLAG_TF];

	// If a timer tick is pending, interrupts are enabled, and no overrides/REP are active,
	// then process the tick and check for new keystrokes
	if (int8_asap && !seg_override_en && !rep_override_en && regs8[FLAG_IF] && !regs8[FLAG_TF])
		pc_interrupt(0xA), int8_asap = 0, KEYBOARD_DRIVER;
}

void VM86::Run()
{
	while (!pause) {
		// Check the finishing condition. Terminates if CS:IP = 0:0
#ifdef MRAM_TEST
		opcode_stream = 16 * regs16[REG_CS] + reg_ip;
#else
		opcode_stream = mem + 16 * regs16[REG_CS] + reg_ip;
#endif
		if (regs16[REG_CS] + reg_ip == 0) pause = 2;
		// Do an actual step
		Step();
	}
}
