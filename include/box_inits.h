#ifndef DOSBOX_INITS_H_
#define DOSBOX_INITS_H_

namespace dosbox {

void LOG_StartUp(void);

void MEM_Init();
void MEM_Clear();

void PAGING_Init();
void PAGING_Clear();

void IO_Init();
void IO_Clear();

void CALLBACK_Init();

void PROGRAMS_Init();

void RENDER_Init();

void VGA_Init();
void VGA_Memory_Clear();

void CPU_Init();
void CPU_Clear();

#if C_FPU
void FPU_Init();
#endif

void DMA_Init();
void DMA_Clear();

void MIXER_Init();
void MIXER_Clear();

void HARDWARE_Init();

void KEYBOARD_Init();

void MOUSE_Init();

void SBLASTER_Init();
void SBLASTER_Clear();

void PCSPEAKER_Init();
void PCSPEAKER_Clear();

void SERIAL_Init();
void SERIAL_Clear();

void PIC_Init();
void PIC_Clear();

void TIMER_Init();
void TIMER_Clear();

void BIOS_Init();
void BIOS_Clear();

void DEBUG_Init();

void CMOS_Init();
void CMOS_Clear();

void MSCDEX_Init();
void MSCDEX_Clear();

void DRIVES_Init();

void CDROM_Image_Init();

void EMS_Init();
void EMS_Clear();

void XMS_Init();
void XMS_Clear();

void DOS_KeyboardLayout_Init();
void DOS_KeyboardLayout_Clear();

void DOS_Init();
void DOS_Clear();

void AUTOEXEC_Init();
void SHELL_Init(void);

void INT10_Init();

#if defined(PCI_FUNCTIONALITY_ENABLED)
void PCI_Init();
#endif

}

#endif
