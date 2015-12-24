// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#ifndef VM86_H_
#define VM86_H_

#include <time.h>
#include <sys/timeb.h>
#include <memory.h>
#include "VM86conf.h"

class VM86 {
protected:
	unsigned char mem[RAM_SIZE], io_ports[IO_PORT_COUNT];
	unsigned char *opcode_stream, *regs8;
	unsigned char i_rm, i_w, i_reg, i_mod, i_mod_size, i_d, i_reg4bit;
	unsigned char raw_opcode_id, xlat_opcode_id, extra;
	unsigned char rep_mode;
	unsigned char seg_override_en, rep_override_en;
	unsigned char trap_flag, int8_asap, scratch_uchar, io_hi_lo, *vid_mem_base, spkr_en;
	unsigned char bios_table_lookup[20][256];
	unsigned short *regs16, reg_ip, seg_override;
	unsigned short file_index, wave_counter;
	unsigned int op_source, op_dest, rm_addr, op_to_addr, op_from_addr;
	unsigned int i_data0, i_data1, i_data2;
	unsigned int scratch_uint, scratch2_uint;
	unsigned int inst_counter, set_flags_type;
	unsigned int GRAPHICS_X, GRAPHICS_Y, pixel_colors[16], vmem_ctr;
	int op_result, disk[NUMVDISKS], scratch_int;
	time_t clock_buf;
	struct timeb ms_clock;
	unsigned short vid_addr_lookup[VIDEO_RAM_SIZE], cga_colors[4];
	int pause;

	void CloseDD();
	char set_CF(int new_CF);
	char set_AF(int new_AF);
	char set_OF(int new_OF);
	char set_AF_OF_arith();
	void make_flags();
	void set_flags(int new_flags);
	void set_opcode(unsigned char opcode);
	char pc_interrupt(unsigned char interrupt_num);
	int AAA_AAS(char which_operation);
	void audio_callback(void *data, unsigned char *stream, int len);
	void IEU();
	void LocalOpcode();

public:
	VM86();
	virtual ~VM86();

	void Reset();
	void Step();
	void Run();
	void Pause();
	int GetState()		const	{ return pause; }
};

// Helper macros

// Decode mod, r_m and reg fields in instruction
#define DECODE_RM_REG scratch2_uint = 4 * !i_mod, \
					  op_to_addr = rm_addr = i_mod < 3 ? SEGREG(seg_override_en ? seg_override : bios_table_lookup[scratch2_uint + 3][i_rm], bios_table_lookup[scratch2_uint][i_rm], regs16[bios_table_lookup[scratch2_uint + 1][i_rm]] + bios_table_lookup[scratch2_uint + 2][i_rm] * i_data1+) : GET_REG_ADDR(i_rm), \
					  op_from_addr = GET_REG_ADDR(i_reg), \
					  i_d && (scratch_uint = op_from_addr, op_from_addr = rm_addr, op_to_addr = scratch_uint)

// Return memory-mapped register location (offset into mem array) for register #reg_id
#define GET_REG_ADDR(reg_id) (REGS_BASE + (i_w ? 2 * reg_id : 2 * reg_id + reg_id / 4 & 7))

// Returns number of top bit in operand (i.e. 8 for 8-bit operands, 16 for 16-bit operands)
#define TOP_BIT 8*(i_w + 1)

// Opcode execution unit helpers
#define OPCODE ;break; case
#define OPCODE_CHAIN ; /* no break */ case

// [I]MUL/[I]DIV/DAA/DAS/ADC/SBB helpers
#define MUL_MACRO(op_data_type,out_regs) (set_opcode(0x10), \
										  out_regs[i_w + 1] = (op_result = CAST(op_data_type)mem[rm_addr] * (op_data_type)*out_regs) >> 16, \
										  regs16[REG_AX] = op_result, \
										  set_OF(set_CF(op_result - (op_data_type)op_result)))
#define DIV_MACRO(out_data_type,in_data_type,out_regs) (scratch_int = CAST(out_data_type)mem[rm_addr]) && !(scratch2_uint = (in_data_type)(scratch_uint = (out_regs[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (out_data_type)scratch2_uint) ? out_regs[i_w+1] = scratch_uint - scratch_int * (*out_regs = scratch2_uint) : pc_interrupt(0)
#define DAA_DAS(op1,op2,mask,min) set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF]) && (op_result = regs8[REG_AL] op1 6, set_CF(regs8[FLAG_CF] || (regs8[REG_AL] op2 scratch2_uint))), \
								  set_CF((((mask & 1 ? scratch2_uint : regs8[REG_AL]) & mask) > min) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] op1 0x60)
#define ADC_SBB_MACRO(a) OP(a##= regs8[FLAG_CF] +), \
						 set_CF(regs8[FLAG_CF] && (op_result == op_dest) || (a op_result < a(int)op_dest)), \
						 set_AF_OF_arith()

// Execute arithmetic/logic operations in emulator memory/registers
#define R_M_OP(dest,op,src) (i_w ? op_dest = CAST(unsigned short)dest, op_result = CAST(unsigned short)dest op (op_source = CAST(unsigned short)src) \
								 : (op_dest = dest, op_result = dest op (op_source = CAST(unsigned char)src)))
#define MEM_OP(dest,op,src) R_M_OP(mem[dest],op,mem[src])
#define OP(op) MEM_OP(op_to_addr,op,op_from_addr)

// Increment or decrement a register #reg_id (usually SI or DI), depending on direction flag and operand size (given by i_w)
#define INDEX_INC(reg_id) (regs16[reg_id] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1))

// Helpers for stack operations
#define R_M_PUSH(a) (i_w = 1, R_M_OP(mem[SEGREG(REG_SS, REG_SP, --)], =, a))
#define R_M_POP(a) (i_w = 1, regs16[REG_SP] += 2, R_M_OP(a, =, mem[SEGREG(REG_SS, REG_SP, -2+)]))

// Convert segment:offset to linear address in emulator memory space
#define SEGREG(reg_seg,reg_ofs,op) 16 * regs16[reg_seg] + (unsigned short)(op regs16[reg_ofs])

// Returns sign bit of an 8-bit or 16-bit operand
#define SIGN_OF(a) (1 & (i_w ? CAST(short)a : a) >> (TOP_BIT - 1))

// Reinterpretation cast
#define CAST(a) *(a*)&

// Keyboard driver for console. This may need changing for UNIX/non-UNIX platforms
#define KEYBOARD_DRIVER read(0, mem + 0x4A6, 1) && (int8_asap = (mem[0x4A6] == 0x1B), pc_interrupt(7))

#endif /* VM86_H_ */
