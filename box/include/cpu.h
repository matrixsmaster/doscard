/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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


#ifndef DOSBOX_CPU_H
#define DOSBOX_CPU_H

#include "dosbox.h" 
#include "regs.h"
#include "mem.h"
#include "abstr_subsys.h"

namespace dosbox {

#define CPU_AUTODETERMINE_NONE		0x00
#define CPU_AUTODETERMINE_CORE		0x01
#define CPU_AUTODETERMINE_CYCLES	0x02

#define CPU_AUTODETERMINE_SHIFT		0x02
#define CPU_AUTODETERMINE_MASK		0x03

#define CPU_CYCLES_LOWER_LIMIT		100

#define CPU_ARCHTYPE_MIXED			0xff
#define CPU_ARCHTYPE_386SLOW		0x30
#define CPU_ARCHTYPE_386FAST		0x35
#define CPU_ARCHTYPE_486OLDSLOW		0x40
#define CPU_ARCHTYPE_486NEWSLOW		0x45
#define CPU_ARCHTYPE_PENTIUMSLOW	0x50

#define CPU_INT_SOFTWARE		0x1
#define CPU_INT_EXCEPTION		0x2
#define CPU_INT_HAS_ERROR		0x4
#define CPU_INT_NOIOPLCHECK		0x8

#define EXCEPTION_UD			6
#define EXCEPTION_TS			10
#define EXCEPTION_NP			11
#define EXCEPTION_SS			12
#define EXCEPTION_GP			13
#define EXCEPTION_PF			14

#define CR0_PROTECTION			0x00000001
#define CR0_MONITORPROCESSOR	0x00000002
#define CR0_FPUEMULATION		0x00000004
#define CR0_TASKSWITCH			0x00000008
#define CR0_FPUPRESENT			0x00000010
#define CR0_PAGING				0x80000000

// *********************************************************************
// Flags
// *********************************************************************

#define lf_var1b lflags.var1.byte[BL_INDEX]
#define lf_var2b lflags.var2.byte[BL_INDEX]
#define lf_resb lflags.res.byte[BL_INDEX]

#define lf_var1w lflags.var1.word[W_INDEX]
#define lf_var2w lflags.var2.word[W_INDEX]
#define lf_resw lflags.res.word[W_INDEX]

#define lf_var1d lflags.var1.dword[DW_INDEX]
#define lf_var2d lflags.var2.dword[DW_INDEX]
#define lf_resd lflags.res.dword[DW_INDEX]

#define SETFLAGSb(FLAGB)													\
{																			\
	SETFLAGBIT(OF,get_OF());												\
	lflags.type=t_UNKNOWN;													\
	CPU_SetFlags(FLAGB,FMASK_NORMAL & 0xff);								\
}

#define SETFLAGSw(FLAGW)													\
{																			\
	lflags.type=t_UNKNOWN;													\
	CPU_SetFlagsw(FLAGW);													\
}

#define SETFLAGSd(FLAGD)													\
{																			\
	lflags.type=t_UNKNOWN;													\
	CPU_SetFlagsd(FLAGD);													\
}

#define LoadCF SETFLAGBIT(CF,get_CF());
#define LoadZF SETFLAGBIT(ZF,get_ZF());
#define LoadSF SETFLAGBIT(SF,get_SF());
#define LoadOF SETFLAGBIT(OF,get_OF());
#define LoadAF SETFLAGBIT(AF,get_AF());

#define TFLG_O		(get_OF())
#define TFLG_NO		(!get_OF())
#define TFLG_B		(get_CF())
#define TFLG_NB		(!get_CF())
#define TFLG_Z		(get_ZF())
#define TFLG_NZ		(!get_ZF())
#define TFLG_BE		(get_CF() || get_ZF())
#define TFLG_NBE	(!get_CF() && !get_ZF())
#define TFLG_S		(get_SF())
#define TFLG_NS		(!get_SF())
#define TFLG_P		(get_PF())
#define TFLG_NP		(!get_PF())
#define TFLG_L		((get_SF()!=0) != (get_OF()!=0))
#define TFLG_NL		((get_SF()!=0) == (get_OF()!=0))
#define TFLG_LE		(get_ZF()  || ((get_SF()!=0) != (get_OF()!=0)))
#define TFLG_NLE	(!get_ZF() && ((get_SF()!=0) == (get_OF()!=0)))


// *********************************************************************
// Descriptor
// *********************************************************************

#define DESC_INVALID				0x00
#define DESC_286_TSS_A				0x01
#define DESC_LDT					0x02
#define DESC_286_TSS_B				0x03
#define DESC_286_CALL_GATE			0x04
#define DESC_TASK_GATE				0x05
#define DESC_286_INT_GATE			0x06
#define DESC_286_TRAP_GATE			0x07

#define DESC_386_TSS_A				0x09
#define DESC_386_TSS_B				0x0b
#define DESC_386_CALL_GATE			0x0c
#define DESC_386_INT_GATE			0x0e
#define DESC_386_TRAP_GATE			0x0f

/* EU/ED Expand UP/DOWN RO/RW Read Only/Read Write NA/A Accessed */
#define DESC_DATA_EU_RO_NA			0x10
#define DESC_DATA_EU_RO_A			0x11
#define DESC_DATA_EU_RW_NA			0x12
#define DESC_DATA_EU_RW_A			0x13
#define DESC_DATA_ED_RO_NA			0x14
#define DESC_DATA_ED_RO_A			0x15
#define DESC_DATA_ED_RW_NA			0x16
#define DESC_DATA_ED_RW_A			0x17

/* N/R Readable  NC/C Confirming A/NA Accessed */
#define DESC_CODE_N_NC_A			0x18
#define DESC_CODE_N_NC_NA			0x19
#define DESC_CODE_R_NC_A			0x1a
#define DESC_CODE_R_NC_NA			0x1b
#define DESC_CODE_N_C_A				0x1c
#define DESC_CODE_N_C_NA			0x1d
#define DESC_CODE_R_C_A				0x1e
#define DESC_CODE_R_C_NA			0x1f


struct S_Descriptor {
#ifdef WORDS_BIGENDIAN
	Bit32u base_0_15	:16;
	Bit32u limit_0_15	:16;
	Bit32u base_24_31	:8;
	Bit32u g			:1;
	Bit32u big			:1;
	Bit32u r			:1;
	Bit32u avl			:1;
	Bit32u limit_16_19	:4;
	Bit32u p			:1;
	Bit32u dpl			:2;
	Bit32u type			:5;
	Bit32u base_16_23	:8;
#else
	Bit32u limit_0_15	:16;
	Bit32u base_0_15	:16;
	Bit32u base_16_23	:8;
	Bit32u type			:5;
	Bit32u dpl			:2;
	Bit32u p			:1;
	Bit32u limit_16_19	:4;
	Bit32u avl			:1;
	Bit32u r			:1;
	Bit32u big			:1;
	Bit32u g			:1;
	Bit32u base_24_31	:8;
#endif
}GCC_ATTRIBUTE(packed);

struct G_Descriptor {
#ifdef WORDS_BIGENDIAN
	Bit32u selector:	16;
	Bit32u offset_0_15	:16;
	Bit32u offset_16_31	:16;
	Bit32u p			:1;
	Bit32u dpl			:2;
	Bit32u type			:5;
	Bit32u reserved		:3;
	Bit32u paramcount	:5;
#else
	Bit32u offset_0_15	:16;
	Bit32u selector		:16;
	Bit32u paramcount	:5;
	Bit32u reserved		:3;
	Bit32u type			:5;
	Bit32u dpl			:2;
	Bit32u p			:1;
	Bit32u offset_16_31	:16;
#endif
} GCC_ATTRIBUTE(packed);

struct TSS_16 {	
    Bit16u back;                 /* Back link to other task */
    Bit16u sp0;				     /* The CK stack pointer */
    Bit16u ss0;					 /* The CK stack selector */
	Bit16u sp1;                  /* The parent KL stack pointer */
    Bit16u ss1;                  /* The parent KL stack selector */
	Bit16u sp2;                  /* Unused */
    Bit16u ss2;                  /* Unused */
    Bit16u ip;                   /* The instruction pointer */
    Bit16u flags;                /* The flags */
    Bit16u ax, cx, dx, bx;       /* The general purpose registers */
    Bit16u sp, bp, si, di;       /* The special purpose registers */
    Bit16u es;                   /* The extra selector */
    Bit16u cs;                   /* The code selector */
    Bit16u ss;                   /* The application stack selector */
    Bit16u ds;                   /* The data selector */
    Bit16u ldt;                  /* The local descriptor table */
} GCC_ATTRIBUTE(packed);

struct TSS_32 {	
    Bit32u back;                /* Back link to other task */
	Bit32u esp0;		         /* The CK stack pointer */
    Bit32u ss0;					 /* The CK stack selector */
	Bit32u esp1;                 /* The parent KL stack pointer */
    Bit32u ss1;                  /* The parent KL stack selector */
	Bit32u esp2;                 /* Unused */
    Bit32u ss2;                  /* Unused */
	Bit32u cr3;                  /* The page directory pointer */
    Bit32u eip;                  /* The instruction pointer */
    Bit32u eflags;               /* The flags */
    Bit32u eax, ecx, edx, ebx;   /* The general purpose registers */
    Bit32u esp, ebp, esi, edi;   /* The special purpose registers */
    Bit32u es;                   /* The extra selector */
    Bit32u cs;                   /* The code selector */
    Bit32u ss;                   /* The application stack selector */
    Bit32u ds;                   /* The data selector */
    Bit32u fs;                   /* And another extra selector */
    Bit32u gs;                   /* ... and another one */
    Bit32u ldt;                  /* The local descriptor table */
} GCC_ATTRIBUTE(packed);

class Descriptor
{
public:
	Descriptor() { saved.fill[0]=saved.fill[1]=0; }

	void Load(PhysPt address);
	void Save(PhysPt address);

	PhysPt GetBase (void) { 
		return (saved.seg.base_24_31<<24) | (saved.seg.base_16_23<<16) | saved.seg.base_0_15; 
	}
	Bitu GetLimit (void) {
		Bitu limit = (saved.seg.limit_16_19<<16) | saved.seg.limit_0_15;
		if (saved.seg.g)	return (limit<<12) | 0xFFF;
		return limit;
	}
	Bitu GetOffset(void) {
		return (saved.gate.offset_16_31 << 16) | saved.gate.offset_0_15;
	}
	Bitu GetSelector(void) {
		return saved.gate.selector;
	}
	Bitu Type(void) {
		return saved.seg.type;
	}
	Bitu Conforming(void) {
		return saved.seg.type & 8;
	}
	Bitu DPL(void) {
		return saved.seg.dpl;
	}
	Bitu Big(void) {
		return saved.seg.big;
	}
public:
	union {
		S_Descriptor seg;
		G_Descriptor gate;
		Bit32u fill[2];
	} saved;
};

class DescriptorTable {
public:
	PhysPt	GetBase			(void)			{ return table_base;	}
	Bitu	GetLimit		(void)			{ return table_limit;	}
	void	SetBase			(PhysPt _base)	{ table_base = _base;	}
	void	SetLimit		(Bitu _limit)	{ table_limit= _limit;	}

	bool GetDescriptor	(Bitu selector, Descriptor& desc) {
		selector&=~7;
		if (selector>=table_limit) return false;
		desc.Load(table_base+(selector));
		return true;
	}
protected:
	PhysPt table_base;
	Bitu table_limit;
};

class GDTDescriptorTable : public DescriptorTable {
public:
	bool GetDescriptor(Bitu selector, Descriptor& desc) {
		Bitu address=selector & ~7;
		if (selector & 4) {
			if (address>=ldt_limit) return false;
			desc.Load(ldt_base+address);
			return true;
		} else {
			if (address>=table_limit) return false;
			desc.Load(table_base+address);
			return true;
		}
	}
	bool SetDescriptor(Bitu selector, Descriptor& desc) {
		Bitu address=selector & ~7;
		if (selector & 4) {
			if (address>=ldt_limit) return false;
			desc.Save(ldt_base+address);
			return true;
		} else {
			if (address>=table_limit) return false;
			desc.Save(table_base+address);
			return true;
		}
	} 
	Bitu SLDT(void)	{
		return ldt_value;
	}
	bool LLDT(Bitu value)	{
		if ((value&0xfffc)==0) {
			ldt_value=0;
			ldt_base=0;
			ldt_limit=0;
			return true;
		}
		Descriptor desc;
		if (!GetDescriptor(value,desc)) return !CPU_PrepareException(EXCEPTION_GP,value);
		if (desc.Type()!=DESC_LDT) return !CPU_PrepareException(EXCEPTION_GP,value);
		if (!desc.saved.seg.p) return !CPU_PrepareException(EXCEPTION_NP,value);
		ldt_base=desc.GetBase();
		ldt_limit=desc.GetLimit();
		ldt_value=value;
		return true;
	}
private:
	PhysPt ldt_base;
	Bitu ldt_limit;
	Bitu ldt_value;
};

class TSS_Descriptor : public Descriptor {
public:
	Bitu IsBusy(void) {
		return saved.seg.type & 2;
	}
	Bitu Is386(void) {
		return saved.seg.type & 8;
	}
	void SetBusy(bool busy) {
		if (busy) saved.seg.type|=2;
		else saved.seg.type&=~2;
	}
};

struct CPUBlock {
	Bitu cpl;							/* Current Privilege */
	Bitu mpl;
	Bitu cr0;
	bool pmode;							/* Is Protected mode enabled */
	GDTDescriptorTable gdt;
	DescriptorTable idt;
	struct {
		Bitu mask,notmask;
		bool big;
	} stack;
	struct {
		bool big;
	} code;
	struct {
		Bitu cs,eip;
		CPU_Decoder * old_decoder;
	} hlt;
	struct {
		Bitu which,error;
	} exception;
	Bits direction;
	bool trap_skip;
	Bit32u drx[8];
	Bit32u trx[8];
};

struct LazyFlags {
    GenReg32 var1,var2,res;
	Bitu type;
	Bitu prev_type;
	Bitu oldcf;
};

//Types of Flag changing instructions
enum {
	t_UNKNOWN=0,
	t_ADDb,t_ADDw,t_ADDd,
	t_ORb,t_ORw,t_ORd,
	t_ADCb,t_ADCw,t_ADCd,
	t_SBBb,t_SBBw,t_SBBd,
	t_ANDb,t_ANDw,t_ANDd,
	t_SUBb,t_SUBw,t_SUBd,
	t_XORb,t_XORw,t_XORd,
	t_CMPb,t_CMPw,t_CMPd,
	t_INCb,t_INCw,t_INCd,
	t_DECb,t_DECw,t_DECd,
	t_TESTb,t_TESTw,t_TESTd,
	t_SHLb,t_SHLw,t_SHLd,
	t_SHRb,t_SHRw,t_SHRd,
	t_SARb,t_SARw,t_SARd,
	t_ROLb,t_ROLw,t_ROLd,
	t_RORb,t_RORw,t_RORd,
	t_RCLb,t_RCLw,t_RCLd,
	t_RCRb,t_RCRw,t_RCRd,
	t_NEGb,t_NEGw,t_NEGd,

	t_DSHLw,t_DSHLd,
	t_DSHRw,t_DSHRd,
	t_MUL,t_DIV,
	t_NOTDONE,
	t_LASTFLAG
};

enum TSwitchType {
	TSwitch_JMP,TSwitch_CALL_INT,TSwitch_IRET
};

typedef Bits (CPU_Decoder)(void);

//static INLINE void CPU_SetFlagsd(Bitu word) {
//	Bitu mask=cpu.cpl ? FMASK_NORMAL : FMASK_ALL;
//	CPU_SetFlags(word,mask);
//}

#define CPU_SetFlagsd(X) { \
		Bitu mask=cpu.cpl ? FMASK_NORMAL : FMASK_ALL; \
		CPU_SetFlags(X,mask); }

//static INLINE void CPU_SetFlagsw(Bitu word) {
//	Bitu mask=(cpu.cpl ? FMASK_NORMAL : FMASK_ALL) & 0xffff;
//	CPU_SetFlags(word,mask);
//}

#define CPU_SetFlagsw(X) { \
		Bitu mask=(cpu.cpl ? FMASK_NORMAL : FMASK_ALL) & 0xffff; \
		CPU_SetFlags(X,mask); }

//class CPU;
class TaskStateSegment;

class CPUSubSystem : public CLSBAbstractSubSystem {
public:
	CPUSubSystem(CDosBox* p);
	~CPUSubSystem();

	void Vinit();
	bool Change_Config();

	void CPU_Core_Full_Init(void);
	void CPU_Core_Normal_Init(void);
	void CPU_Core_Simple_Init(void);
	Bits CPU_Core_Normal_Run(void);
	Bits CPU_Core_Normal_Trap_Run(void);
	Bits CPU_Core_Simple_Run(void);
	Bits CPU_Core_Full_Run(void);

	void CPU_Enable_SkipAutoAdjust(void);
	void CPU_Disable_SkipAutoAdjust(void);
	void CPU_Reset_AutoAdjust(void);

	bool CPU_LLDT(Bitu selector);
	bool CPU_LTR(Bitu selector);
	void CPU_LIDT(Bitu limit,Bitu base);
	void CPU_LGDT(Bitu limit,Bitu base);

	Bitu CPU_STR(void);
	Bitu CPU_SLDT(void);
	Bitu CPU_SIDT_base(void);
	Bitu CPU_SIDT_limit(void);
	Bitu CPU_SGDT_base(void);
	Bitu CPU_SGDT_limit(void);

	void CPU_ARPL(Bitu & dest_sel,Bitu src_sel);
	void CPU_LAR(Bitu selector,Bitu & ar);
	void CPU_LSL(Bitu selector,Bitu & limit);

	void CPU_SET_CRX(Bitu cr,Bitu value);
	bool CPU_WRITE_CRX(Bitu cr,Bitu value);
	Bitu CPU_GET_CRX(Bitu cr);
	bool CPU_READ_CRX(Bitu cr,Bit32u & retvalue);

	bool CPU_WRITE_DRX(Bitu dr,Bitu value);
	bool CPU_READ_DRX(Bitu dr,Bit32u & retvalue);

	bool CPU_WRITE_TRX(Bitu dr,Bitu value);
	bool CPU_READ_TRX(Bitu dr,Bit32u & retvalue);

	Bitu CPU_SMSW(void);
	bool CPU_LMSW(Bitu word);

	void CPU_VERR(Bitu selector);
	void CPU_VERW(Bitu selector);

	void CPU_JMP(bool use32,Bitu selector,Bitu offset,Bitu oldeip);
	void CPU_CALL(bool use32,Bitu selector,Bitu offset,Bitu oldeip);
	void CPU_RET(bool use32,Bitu bytes,Bitu oldeip);
	void CPU_IRET(bool use32,Bitu oldeip);
	void CPU_HLT(Bitu oldeip);

	bool CPU_POPF(Bitu use32);
	bool CPU_PUSHF(Bitu use32);
	bool CPU_CLI(void);
	bool CPU_STI(void);

	bool CPU_IO_Exception(Bitu port,Bitu size);
	void CPU_RunException(void);

	void CPU_ENTER(bool use32,Bitu bytes,Bitu level);
	void CPU_Interrupt(Bitu num,Bitu type,Bitu oldeip);

	bool CPU_PrepareException(Bitu which,Bitu error);
	void CPU_Exception(Bitu which,Bitu error=0);

	bool CPU_SetSegGeneral(SegNames seg,Bitu value);
	bool CPU_PopSeg(SegNames seg,bool use32);

	bool CPU_CPUID(void);
	Bitu CPU_Pop16(void);
	Bitu CPU_Pop32(void);
	void CPU_Push16(Bitu value);
	void CPU_Push32(Bitu value);

	void CPU_SetFlags(Bitu word,Bitu mask);
	//Flag Handling
	Bit32u get_CF(void);
	Bit32u get_AF(void);
	Bit32u get_ZF(void);
	Bit32u get_SF(void);
	Bit32u get_OF(void);
	Bit32u get_PF(void);

	Bitu FillFlags(void);
	void FillFlagsNoCFOF(void);
	void DestroyConditionFlags(void);

	inline void CPU_HW_Interrupt(Bitu num) {
		CPU_Interrupt(num,0,reg_eip); }

	inline void CPU_SW_Interrupt(Bitu num,Bitu oldeip) {
		CPU_Interrupt(num,CPU_INT_SOFTWARE,oldeip); }

	inline void CPU_SW_Interrupt_NoIOPLCheck(Bitu num,Bitu oldeip) {
		CPU_Interrupt(num,CPU_INT_SOFTWARE|CPU_INT_NOIOPLCHECK,oldeip); }

	inline PhysPt SegPhys(SegNames index) {
		return Segs.phys[index]; }

	inline Bit16u SegValue(SegNames index) {
		return (Bit16u)Segs.val[index]; }

	inline RealPt RealMakeSeg(SegNames index,Bit16u off) {
		return RealMake(SegValue(index),off); }

	inline void SegSet16(Bitu index,Bit16u val) {
		Segs.val[index]=val; Segs.phys[index]=val << 4; }

	Bit32s CPU_Cycles;
	Bit32s CPU_CycleLeft;
	Bit32s CPU_CycleMax;
	Bit32s CPU_OldCycleMax;
	Bit32s CPU_CyclePercUsed;
	Bit32s CPU_CycleLimit;
	Bit64s CPU_IODelayRemoved;
	bool CPU_CycleAutoAdjust;
	bool CPU_SkipCycleAutoAdjust;
	Bitu CPU_AutoDetermineMode;
	Bitu CPU_ArchitectureType;
	Bitu CPU_PrefetchQueueSize;
	Bitu CPU_extflags_toggle;
	CPU_Decoder * cpudecoder;
	Bit16u parity_lookup[256];
	CPU_Regs cpu_regs;
	CPUBlock cpu;
	Segments Segs;
	LazyFlags lflags;
private:
//	CPU* pCPU;
	bool inited;
	bool printed_cycles_auto_info;
	Bit8u lastint;
	TaskStateSegment cpu_tss;
};

} //namespace dosbox

#endif
