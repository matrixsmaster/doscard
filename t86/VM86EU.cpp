// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include <unistd.h>
#include "VM86.h"

// Instruction execution unit
void VM86::IEU()
{
	switch (xlat_opcode_id)
	{
		OPCODE_CHAIN 0: // Conditional jump (JAE, JNAE, etc.)
			// i_w is the invert flag, e.g. i_w == 1 means JNAE, whereas i_w == 0 means JAE
			scratch_uchar = raw_opcode_id / 2 & 7;
			reg_ip += (char)i_data0 * (i_w ^ (regs8[lookup_table[TABLE_COND_JUMP_DECODE_A][scratch_uchar]] || regs8[lookup_table[TABLE_COND_JUMP_DECODE_B][scratch_uchar]] || regs8[lookup_table[TABLE_COND_JUMP_DECODE_C][scratch_uchar]] ^ regs8[lookup_table[TABLE_COND_JUMP_DECODE_D][scratch_uchar]]))
		OPCODE 1: // MOV reg, imm
			i_w = !!(raw_opcode_id & 8);
			R_M_OP(mem[GET_REG_ADDR(i_reg4bit)], =, i_data0)
		OPCODE 3: // PUSH regs16
			R_M_PUSH(regs16[i_reg4bit])
		OPCODE 4: // POP regs16
			R_M_POP(regs16[i_reg4bit])
		OPCODE 2: // INC|DEC regs16
			i_w = 1;
			i_d = 0;
			i_reg = i_reg4bit;
			DecodeRM_REG();
			i_reg = extra
		OPCODE_CHAIN 5: // INC|DEC|JMP|CALL|PUSH
			if (i_reg < 2) // INC|DEC
				MEM_OP(op_from_addr, += 1 - 2 * i_reg +, REGS_BASE + 2 * REG_ZERO),
				op_source = 1,
				set_AF_OF_arith(),
				set_OF(op_dest + 1 - i_reg == 1 << (TOP_BIT - 1)),
				(xlat_opcode_id == 5) && (set_opcode(0x10), 0); // Decode like ADC
			else if (i_reg != 6) // JMP|CALL
				(i_reg - 3) || R_M_PUSH(regs16[REG_CS]), // CALL (far)
				(i_reg & 2) && R_M_PUSH(reg_ip + 2 + i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6)), // CALL (near or far)
				(i_reg & 1) && (regs16[REG_CS] = CAST(short)mem[op_from_addr + 2]), // JMP|CALL (far)
				R_M_OP(reg_ip, =, mem[op_from_addr]),
				set_opcode(0x9A); // Decode like CALL
			else // PUSH
				R_M_PUSH(mem[rm_addr])
		OPCODE 6: // TEST r/m, imm16 / NOT|NEG|MUL|IMUL|DIV|IDIV reg
			op_to_addr = op_from_addr;

			switch (i_reg)
			{
				OPCODE_CHAIN 0: // TEST
					set_opcode(0x20); // Decode like AND
					reg_ip += i_w + 1;
					R_M_OP(mem[op_to_addr], &, i_data2)
				OPCODE 2: // NOT
					OP(=~)
				OPCODE 3: // NEG
					OP(=-);
					op_dest = 0;
					set_opcode(0x28); // Decode like SUB
					set_CF(op_result > op_dest)
				OPCODE 4: // MUL
					MUL();
//					i_w ? MUL_MACRO(unsigned short, regs16) : MUL_MACRO(unsigned char, regs8)
				OPCODE 5: // IMUL
					IMUL();
//					i_w ? MUL_MACRO(short, regs16) : MUL_MACRO(char, regs8)
				OPCODE 6: // DIV
					DIV();
//					i_w ? DIV_MACRO(unsigned short, unsigned, regs16) : DIV_MACRO(unsigned char, unsigned short, regs8)
				OPCODE 7: // IDIV
					IDIV();
//					i_w ? DIV_MACRO(short, int, regs16) : DIV_MACRO(char, short, regs8);
			}
		OPCODE 7: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP AL/AX, immed
			rm_addr = REGS_BASE;
			i_data2 = i_data0;
			i_mod = 3;
			i_reg = extra;
			reg_ip--;
		OPCODE_CHAIN 8: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP reg, immed
			op_to_addr = rm_addr;
			regs16[REG_SCRATCH] = (i_d |= !i_w) ? (char)i_data2 : i_data2;
			op_from_addr = REGS_BASE + 2 * REG_SCRATCH;
			reg_ip += !i_d + 1;
			set_opcode(0x08 * (extra = i_reg));
		OPCODE_CHAIN 9: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP|MOV reg, r/m
			switch (extra)
			{
				OPCODE_CHAIN 0: // ADD
					OP(+=),
					set_CF(op_result < op_dest)
				OPCODE 1: // OR
					OP(|=)
				OPCODE 2: // ADC
					ADC_SBB_MACRO(+)
				OPCODE 3: // SBB
					ADC_SBB_MACRO(-)
				OPCODE 4: // AND
					OP(&=)
				OPCODE 5: // SUB
					OP(-=),
					set_CF(op_result > op_dest)
				OPCODE 6: // XOR
					OP(^=)
				OPCODE 7: // CMP
					OP(-),
					set_CF(op_result > op_dest)
				OPCODE 8: // MOV
					OP(=);
			}
		OPCODE 10: // MOV sreg, r/m | POP r/m | LEA reg, r/m
			if (!i_w) // MOV
				i_w = 1,
				i_reg += 8,
				DecodeRM_REG(),
				OP(=);
			else if (!i_d) // LEA
				seg_override_en = 1,
				seg_override = REG_ZERO,
				DecodeRM_REG(),
				R_M_OP(mem[op_from_addr], =, rm_addr);
			else // POP
				R_M_POP(mem[rm_addr])
		OPCODE 11: // MOV AL/AX, [loc]
			i_mod = i_reg = 0;
			i_rm = 6;
			i_data1 = i_data0;
			DecodeRM_REG();
			MEM_OP(op_from_addr, =, op_to_addr)
		OPCODE 12: // ROL|ROR|RCL|RCR|SHL|SHR|???|SAR reg/mem, 1/CL/imm (80186)
			scratch2_uint = SIGN_OF(mem[rm_addr]),
			scratch_uint = extra ? // xxx reg/mem, imm
				++reg_ip,
				(char)i_data1
			: // xxx reg/mem, CL
				i_d
					? 31 & regs8[REG_CL]
			: // xxx reg/mem, 1
				1;
			if (scratch_uint)
			{
				if (i_reg < 4) // Rotate operations
					scratch_uint %= i_reg / 2 + TOP_BIT,
					R_M_OP(scratch2_uint, =, mem[rm_addr]);
				if (i_reg & 1) // Rotate/shift right operations
					R_M_OP(mem[rm_addr], >>=, scratch_uint);
				else // Rotate/shift left operations
					R_M_OP(mem[rm_addr], <<=, scratch_uint);
				if (i_reg > 3) // Shift operations
					set_opcode(0x10); // Decode like ADC
				if (i_reg > 4) // SHR or SAR
					set_CF(op_dest >> (scratch_uint - 1) & 1);
			}

			switch (i_reg)
			{
				OPCODE_CHAIN 0: // ROL
					R_M_OP(mem[rm_addr], += , scratch2_uint >> (TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result) ^ set_CF(op_result & 1))
				OPCODE 1: // ROR
					scratch2_uint &= (1 << scratch_uint) - 1,
					R_M_OP(mem[rm_addr], += , scratch2_uint << (TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result * 2) ^ set_CF(SIGN_OF(op_result)))
				OPCODE 2: // RCL
					R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (scratch_uint - 1)) + , scratch2_uint >> (1 + TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result) ^ set_CF(scratch2_uint & 1 << (TOP_BIT - scratch_uint)))
				OPCODE 3: // RCR
					R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (TOP_BIT - scratch_uint)) + , scratch2_uint << (1 + TOP_BIT - scratch_uint));
					set_CF(scratch2_uint & 1 << (scratch_uint - 1));
					set_OF(SIGN_OF(op_result) ^ SIGN_OF(op_result * 2))
				OPCODE 4: // SHL
					set_OF(SIGN_OF(op_result) ^ set_CF(SIGN_OF(op_dest << (scratch_uint - 1))))
				OPCODE 5: // SHR
					set_OF(SIGN_OF(op_dest))
				OPCODE 7: // SAR
					scratch_uint < TOP_BIT || set_CF(scratch2_uint);
					set_OF(0);
					R_M_OP(mem[rm_addr], +=, scratch2_uint *= ~(((1 << TOP_BIT) - 1) >> scratch_uint));
			}
		OPCODE 13: // LOOPxx|JCZX
			scratch_uint = !!--regs16[REG_CX];

			switch(i_reg4bit)
			{
				OPCODE_CHAIN 0: // LOOPNZ
					scratch_uint &= !regs8[FLAG_ZF]
				OPCODE 1: // LOOPZ
					scratch_uint &= regs8[FLAG_ZF]
				OPCODE 3: // JCXXZ
					scratch_uint = !++regs16[REG_CX];
			}
			reg_ip += scratch_uint*(char)i_data0
		OPCODE 14: // JMP | CALL short/near
			reg_ip += 3 - i_d;
			if (!i_w)
			{
				if (i_d) // JMP far
					reg_ip = 0,
					regs16[REG_CS] = i_data2;
				else // CALL
					R_M_PUSH(reg_ip);
			}
			reg_ip += i_d && i_w ? (char)i_data0 : i_data0
		OPCODE 15: // TEST reg, r/m
			MEM_OP(op_from_addr, &, op_to_addr)
		OPCODE 16: // XCHG AX, regs16
			i_w = 1;
			op_to_addr = REGS_BASE;
			op_from_addr = GET_REG_ADDR(i_reg4bit);
		OPCODE_CHAIN 24: // NOP|XCHG reg, r/m
			if (op_to_addr != op_from_addr)
				OP(^=),
				MEM_OP(op_from_addr, ^=, op_to_addr),
				OP(^=)
		OPCODE 17: // MOVSx (extra=0)|STOSx (extra=1)|LODSx (extra=2)
			scratch2_uint = seg_override_en ? seg_override : REG_DS;

			for (scratch_uint = rep_override_en ? regs16[REG_CX] : 1; scratch_uint; scratch_uint--)
			{
				MEM_OP(extra < 2 ? SEGREG(REG_ES, REG_DI,) : REGS_BASE, =, extra & 1 ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,)),
				extra & 1 || INDEX_INC(REG_SI),
				extra & 2 || INDEX_INC(REG_DI);
			}

			if (rep_override_en)
				regs16[REG_CX] = 0
		OPCODE 18: // CMPSx (extra=0)|SCASx (extra=1)
			scratch2_uint = seg_override_en ? seg_override : REG_DS;

			if ((scratch_uint = rep_override_en ? regs16[REG_CX] : 1))
			{
				for (; scratch_uint; rep_override_en || scratch_uint--)
				{
					MEM_OP(extra ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,), -, SEGREG(REG_ES, REG_DI,)),
					extra || INDEX_INC(REG_SI),
					INDEX_INC(REG_DI), rep_override_en && !(--regs16[REG_CX] && (!op_result == rep_mode)) && (scratch_uint = 0);
				}

				set_flags_type = FLAGS_UPDATE_SZP | FLAGS_UPDATE_AO_ARITH; // Funge to set SZP/AO flags
				set_CF(op_result > op_dest);
			}
		OPCODE 19: // RET|RETF|IRET
			i_d = i_w;
			R_M_POP(reg_ip);
			if (extra) // IRET|RETF|RETF imm16
				R_M_POP(regs16[REG_CS]);
			if (extra & 2) // IRET
				set_flags(R_M_POP(scratch_uint));
			else if (!i_d) // RET|RETF imm16
				regs16[REG_SP] += i_data0
		OPCODE 20: // MOV r/m, immed
			R_M_OP(mem[op_from_addr], =, i_data2)
		OPCODE 21: // IN AL/AX, DX/imm8
			io_ports[0x20] = 0; // PIC EOI
			io_ports[0x42] = --io_ports[0x40]; // PIT channel 0/2 read placeholder
			io_ports[0x3DA] ^= 9; // CGA refresh
			scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
			scratch_uint == 0x60 && (io_ports[0x64] = 0); // Scancode read flag
			scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 7) && (io_ports[0x3D5] = ((mem[0x49E]*80 + mem[0x49D] + CAST(short)mem[0x4AD]) & (io_ports[0x3D4] & 1 ? 0xFF : 0xFF00)) >> (io_ports[0x3D4] & 1 ? 0 : 8)); // CRT cursor position
			R_M_OP(regs8[REG_AL], =, io_ports[scratch_uint]);
		OPCODE 22: // OUT DX/imm8, AL/AX
			scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
			R_M_OP(io_ports[scratch_uint], =, regs8[REG_AL]);
			scratch_uint == 0x61 && (io_hi_lo = 0, spkr_en |= regs8[REG_AL] & 3); // Speaker control
			(scratch_uint == 0x40 || scratch_uint == 0x42) && (io_ports[0x43] & 6) && (mem[0x469 + scratch_uint - (io_hi_lo ^= 1)] = regs8[REG_AL]); // PIT rate programming
#ifndef NO_GRAPHICS
//			scratch_uint == 0x43 && (io_hi_lo = 0, regs8[REG_AL] >> 6 == 2) && (SDL_PauseAudio((regs8[REG_AL] & 0xF7) != 0xB6), 0); // Speaker enable
#endif
			scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 6) && (mem[0x4AD + !(io_ports[0x3D4] & 1)] = regs8[REG_AL]); // CRT video RAM start offset
			scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 7) && (scratch2_uint = ((mem[0x49E]*80 + mem[0x49D] + CAST(short)mem[0x4AD]) & (io_ports[0x3D4] & 1 ? 0xFF00 : 0xFF)) + (regs8[REG_AL] << (io_ports[0x3D4] & 1 ? 0 : 8)) - CAST(short)mem[0x4AD], mem[0x49D] = scratch2_uint % 80, mem[0x49E] = scratch2_uint / 80); // CRT cursor position
			scratch_uint == 0x3B5 && io_ports[0x3B4] == 1 && (GRAPHICS_X = regs8[REG_AL] * 16); // Hercules resolution reprogramming. Defaults are set in the BIOS
			scratch_uint == 0x3B5 && io_ports[0x3B4] == 6 && (GRAPHICS_Y = regs8[REG_AL] * 4);
		OPCODE 23: // REPxx
			rep_override_en = 2;
			rep_mode = i_w;
			seg_override_en && seg_override_en++
		OPCODE 25: // PUSH reg
			R_M_PUSH(regs16[extra])
		OPCODE 26: // POP reg
			R_M_POP(regs16[extra])
		OPCODE 27: // xS: segment overrides
			seg_override_en = 2;
			seg_override = extra;
			rep_override_en && rep_override_en++
		OPCODE 28: // DAA/DAS
			i_w = 0;
//			extra ? DAA_DAS(-=, >=, 0xFF, 0x99) : DAA_DAS(+=, <, 0xF0, 0x90) // extra = 0 for DAA, 1 for DAS
			extra ? DAS() : DAA();
		OPCODE 29: // AAA/AAS
			op_result = AAA_AAS(extra - 1)
		OPCODE 30: // CBW
			regs8[REG_AH] = -SIGN_OF(regs8[REG_AL])
		OPCODE 31: // CWD
			regs16[REG_DX] = -SIGN_OF(regs16[REG_AX])
		OPCODE 32: // CALL FAR imm16:imm16
			R_M_PUSH(regs16[REG_CS]);
			R_M_PUSH(reg_ip + 5);
			regs16[REG_CS] = i_data2;
			reg_ip = i_data0
		OPCODE 33: // PUSHF
			make_flags();
			R_M_PUSH(scratch_uint)
		OPCODE 34: // POPF
			set_flags(R_M_POP(scratch_uint))
		OPCODE 35: // SAHF
			make_flags();
			set_flags((scratch_uint & 0xFF00) + regs8[REG_AH])
		OPCODE 36: // LAHF
			make_flags(),
			regs8[REG_AH] = scratch_uint
		OPCODE 37: // LES|LDS reg, r/m
			i_w = i_d = 1;
			DecodeRM_REG();
			OP(=);
			MEM_OP(REGS_BASE + extra, =, rm_addr + 2)
		OPCODE 38: // INT 3
			++reg_ip;
			pc_interrupt(3)
		OPCODE 39: // INT imm8
			reg_ip += 2;
			pc_interrupt(i_data0)
		OPCODE 40: // INTO
			++reg_ip;
			regs8[FLAG_OF] && pc_interrupt(4)
		OPCODE 41: // AAM
			if (i_data0 &= 0xFF)
				regs8[REG_AH] = regs8[REG_AL] / i_data0,
				op_result = regs8[REG_AL] %= i_data0;
			else // Divide by zero
				pc_interrupt(0)
		OPCODE 42: // AAD
			i_w = 0;
			regs16[REG_AX] = op_result = 0xFF & regs8[REG_AL] + i_data0 * regs8[REG_AH]
		OPCODE 43: // SALC
			regs8[REG_AL] = -regs8[FLAG_CF]
		OPCODE 44: // XLAT
			regs8[REG_AL] = mem[SEGREG(seg_override_en ? seg_override : REG_DS, REG_BX, regs8[REG_AL] +)]
		OPCODE 45: // CMC
			regs8[FLAG_CF] ^= 1
		OPCODE 46: // CLC|STC|CLI|STI|CLD|STD
			regs8[extra / 2] = extra & 1
		OPCODE 47: // TEST AL/AX, immed
			R_M_OP(regs8[REG_AL], &, i_data0)
		OPCODE 48: // Emulator-specific 0F xx opcodes
			LocalOpcode();
	}
}
