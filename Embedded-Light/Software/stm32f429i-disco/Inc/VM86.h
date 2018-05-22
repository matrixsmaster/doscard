// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2018
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#ifndef VM86_H_
#define VM86_H_

#include <deque>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "mthreads.h"
#include "VM86conf.h"

class VM86
{
protected:
    uint8_t *mem, *io_ports;
	uint8_t *opcode_stream,*regs8;
	uint16_t *regs16;

	uint8_t i_rm = 0, i_w = 0, i_reg = 0, i_mod = 0, i_mod_size = 0, i_d = 0, i_reg4bit = 0;
	uint8_t raw_opcode_id = 0, xlat_opcode_id = 0, extra = 0;
	uint8_t rep_mode = 0;
	uint8_t seg_override_en = 0, rep_override_en = 0;
	uint8_t trap_flag = 0, int8_asap = 0, scratch_uchar = 0, io_hi_lo = 0, *vid_mem_base = 0, spkr_en = 0;
	uint16_t reg_ip = 0, seg_override = 0;
	uint16_t file_index = 0, wave_counter = 0;
	uint32_t op_source = 0, op_dest = 0, rm_addr = 0, op_to_addr = 0, op_from_addr = 0;
	uint32_t i_data0 = 0, i_data1 = 0, i_data2 = 0;
	uint32_t scratch_uint = 0, scratch2_uint = 0;
	uint32_t inst_counter = 0, set_flags_type = 0;
	uint32_t graphics_x, graphics_y, pixel_colors[16], vmem_ctr;
	int op_result, disk[NUMVDISKS], scratch_int;
//	time_t clock_buf;
//	struct timeb ms_clock;
	uint16_t *vid_addr_lookup;
	uint16_t cga_colors[4];
	int pause;

	minmutex io_lock;
	std::deque<int> output_chars,input_chars;

	void OpenDD();
	void CloseDD();
	char set_CF(int new_CF);
	char set_AF(int new_AF);
	char set_OF(int new_OF);
	char set_AF_OF_arith();
	void make_flags();
	void set_flags(int new_flags);
	void set_opcode(uint8_t opcode);
	char pc_interrupt(uint8_t interrupt_num);
	int AAA_AAS(char which_operation);
	void DecodeRM_REG();
	void MUL();
	void IMUL();
	void DIV();
	void IDIV();
	void DAA();
	void DAS();
	void IEU();
	void LocalOpcode();

	void PushOutput(int ch);
	bool PullInput(int &ch);

public:
	VM86(uint32_t base_addr);
	virtual ~VM86();

	void Reset();
	void Step();
	int FullStep();
	void Run();

	void setPause(int p)		{ pause = p; }
	int getPause() const		{ return pause; }

	void PushEvent(int key);
	std::string PullTextOutput();
	void getResolution(int &w, int &h) const;
	bool PullVideoData(void *data, int len);
	void PullAudioData(void *data, uint8_t *stream, int len);
};

#endif /* VM86_H_ */
