// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#ifndef VM86CONF_H_
#define VM86CONF_H_

//#define FILENAME_LENGTH 128
//#define MRAM_TEST 1
#define USE_RAW_OUTPUT 1
#define USRIO 1

#define TIMESTEP 5

// Emulator system constants
#define IO_PORT_COUNT	0x10000
#define RAM_SIZE 		0x10FFF0
#define REGS_BASE		0xF0000
#define VIDEO_RAM_SIZE	0x10000
#define NUMVDISKS		0x2

// Graphics/timer/keyboard update delays (explained later)
#ifndef GRAPHICS_UPDATE_DELAY
#define GRAPHICS_UPDATE_DELAY 360000
#endif
#define KEYBOARD_TIMER_UPDATE_DELAY 20000

// 16-bit register decodes
#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7

#define REG_ES 8
#define REG_CS 9
#define REG_SS 10
#define REG_DS 11

#define REG_ZERO 12
#define REG_SCRATCH 13

// 8-bit register decodes
#define REG_AL 0
#define REG_AH 1
#define REG_CL 2
#define REG_CH 3
#define REG_DL 4
#define REG_DH 5
#define REG_BL 6
#define REG_BH 7

// FLAGS register decodes
#define FLAG_CF 40
#define FLAG_PF 41
#define FLAG_AF 42
#define FLAG_ZF 43
#define FLAG_SF 44
#define FLAG_TF 45
#define FLAG_IF 46
#define FLAG_DF 47
#define FLAG_OF 48

// Lookup tables in the BIOS binary
#define TABLE_XLAT_OPCODE 8
#define TABLE_XLAT_SUBFUNCTION 9
#define TABLE_STD_FLAGS 10
#define TABLE_PARITY_FLAG 11
#define TABLE_BASE_INST_SIZE 12
#define TABLE_I_W_SIZE 13
#define TABLE_I_MOD_SIZE 14
#define TABLE_COND_JUMP_DECODE_A 15
#define TABLE_COND_JUMP_DECODE_B 16
#define TABLE_COND_JUMP_DECODE_C 17
#define TABLE_COND_JUMP_DECODE_D 18
#define TABLE_FLAGS_BITFIELDS 19

// Bitfields for TABLE_STD_FLAGS values
#define FLAGS_UPDATE_SZP 1
#define FLAGS_UPDATE_AO_ARITH 2
#define FLAGS_UPDATE_OC_LOGIC 4

static const unsigned short cga_colors_table[] = {0 /* Black */, 0x1F1F /* Cyan */, 0xE3E3 /* Magenta */, 0xFFFF /* White */};

static const unsigned char lookup_table[20][256] = {
	/* Table 0: R/M mode 1/2 "register 1" lookup */
	{ 3, 3, 5, 5, 6, 7, 5, 3 },
	/* Table 1: R/M mode 1/2 "register 2" lookup */
	{ 6, 7, 6, 7, 12, 12, 12, 12 },
	/* Table 2: R/M mode 1/2 "DISP multiplier" lookup */
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	/* Table 3: R/M mode 1/2 "default segment" lookup */
	{ 11, 11, 10, 10, 11, 11, 10, 11 },
	/* Table 4: R/M mode 0 "register 1" lookup */
	{ 3, 3, 5, 5, 6, 7, 12, 3 },
	/* Table 5: R/M mode 0 "register 2" lookup */
	{ 6, 7, 6, 7, 12, 12, 12, 12 },
	/* Table 6: R/M mode 0 "DISP multiplier" lookup */
	{ 0, 0, 0, 0, 0, 0, 1, 0 },
	/* Table 7: R/M mode 0 "default segment" lookup */
	{ 11, 11, 10, 10, 11, 11, 11, 11 },
	/* Table 8: Translation of raw opcode index ("Raw ID") to function number ("Xlat'd ID") */
	{ 9, 9, 9, 9, 7, 7, 25, 26, 9, 9, 9, 9, 7, 7, 25, 48, 9, 9, 9, 9, 7, 7, 25, 26, 9, 9, 9, 9, 7, 7, 25, 26, 9, 9, 9, 9, 7, 7, 27, 28, 9, 9, 9, 9, 7, 7, 27, 28, 9, 9, 9, 9, 7, 7, 27, 29, 9, 9, 9, 9, 7, 7, 27, 29, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 51, 54, 52, 52, 52, 52, 52, 52, 55, 55, 55, 55, 52, 52, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 15, 15, 24, 24, 9, 9, 9, 9, 10, 10, 10, 10, 16, 16, 16, 16, 16, 16, 16, 16, 30, 31, 32, 53, 33, 34, 35, 36, 11, 11, 11, 11, 17, 17, 18, 18, 47, 47, 17, 17, 17, 17, 18, 18, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 12, 12, 19, 19, 37, 37, 20, 20, 49, 50, 19, 19, 38, 39, 40, 19, 12, 12, 12, 12, 41, 42, 43, 44, 53, 53, 53, 53, 53, 53, 53, 53, 13, 13, 13, 13, 21, 21, 22, 22, 14, 14, 14, 14, 21, 21, 22, 22, 53, 0, 23, 23, 53, 45, 6, 6, 46, 46, 46, 46, 46, 46, 5, 5 },
	/* Table 9: Translation of Raw ID to Extra Data */
	{ 0, 0, 0, 0, 0, 0, 8, 8, 1, 1, 1, 1, 1, 1, 9, 36, 2, 2, 2, 2, 2, 2, 10, 10, 3, 3, 3, 3, 3, 3, 11, 11, 4, 4, 4, 4, 4, 4, 8, 0, 5, 5, 5, 5, 5, 5, 9, 1, 6, 6, 6, 6, 6, 6, 10, 2, 7, 7, 7, 7, 7, 7, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 16, 22, 0, 0, 0, 0, 1, 1, 0, 255, 48, 2, 0, 0, 0, 0, 255, 255, 40, 11, 3, 3, 3, 3, 3, 3, 3, 3, 43, 43, 43, 43, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 21, 0, 0, 2, 40, 21, 21, 80, 81, 92, 93, 94, 95, 0, 0 },
	/* Table 10: How each Raw ID sets the flags (bit 1 = sets SZP, bit 2 = sets AF/OF for arithmetic, bit 3 = sets OF/CF for logic) */
	{ 3, 3, 3, 3, 3, 3, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 5, 5, 5, 5, 5, 5, 0, 1, 3, 3, 3, 3, 3, 3, 0, 1, 5, 5, 5, 5, 5, 5, 0, 1, 3, 3, 3, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* Table 11: Parity flag loop-up table (256 entries) */
	{ 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 },
	/* Table 12: Translation of Raw ID to base instruction size (bytes) */
	{ 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 0, 0, 2, 2, 2, 2, 4, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2 },
	/* Table 13: Translation of Raw ID to i_w size adder yes/no */
	{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* Table 14: Translation of Raw ID to i_mod size adder yes/no */
	{ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
	/* Table 15: Jxx decode table A */
	{ 48, 40, 43, 40, 44, 41, 49, 49 },
	/* Table 16: Jxx decode table B */
	{ 49, 49, 49, 43, 49, 49, 49, 43 },
	/* Table 17: Jxx decode table C */
	{ 49, 49, 49, 49, 49, 49, 44, 44 },
	/* Table 18: Jxx decode table D */
	{ 49, 49, 49, 49, 49, 49, 48, 48 },
	/* Table 19: FLAGS multipliers */
	{ 0, 2, 4, 6, 7, 8, 9, 10, 11 }
};

typedef unsigned char uch;

#endif /* VM86CONF_H_ */
