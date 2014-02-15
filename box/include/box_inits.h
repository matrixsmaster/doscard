#ifndef DOSBOX_INITS_H_
#define DOSBOX_INITS_H_

namespace dosbox {

//void MSG_Init(Section_prop *);
void LOG_StartUp(void);

void MEM_Init(Section *);
void MEM_Clear();

void PAGING_Init(Section *);
void PAGING_Clear();

void IO_Init(Section * );
void IO_Clear();

void CALLBACK_Init(Section*);

void PROGRAMS_Init(Section*);

void RENDER_Init(Section*);

void VGA_Init(Section*);
void VGA_Memory_Clear();

void CPU_Init(Section*);
void CPU_Clear();

#if C_FPU
void FPU_Init(Section*);
#endif

void DMA_Init(Section*);
void DMA_Clear();

void MIXER_Init(Section*);
void MIXER_Clear();

void HARDWARE_Init(Section*);

void KEYBOARD_Init(Section*);

void MOUSE_Init(Section*);

void SBLASTER_Init(Section*);
void SBLASTER_Clear();

void PCSPEAKER_Init(Section*);
void PCSPEAKER_Clear();

void SERIAL_Init(Section*);
void SERIAL_Clear();

void PIC_Init(Section*);
void PIC_Clear();

void TIMER_Init(Section*);
void TIMER_Clear();

void BIOS_Init(Section*);
void BIOS_Clear();

void DEBUG_Init(Section*);

void CMOS_Init(Section*);
void CMOS_Clear();

void MSCDEX_Init(Section*);
void MSCDEX_Clear();

void DRIVES_Init(Section*);

void CDROM_Image_Init(Section*);

void EMS_Init(Section*);
void EMS_Clear();

void XMS_Init(Section*);
void XMS_Clear();

void DOS_KeyboardLayout_Init(Section*);
void DOS_KeyboardLayout_Clear();

void DOS_Init(Section*);
void DOS_Clear();

void AUTOEXEC_Init(Section*);
void SHELL_Init(void);

void INT10_Init(Section*);

#if defined(PCI_FUNCTIONALITY_ENABLED)
void PCI_Init(Section*);
#endif

}

#endif
