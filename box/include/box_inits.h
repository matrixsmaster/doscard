#ifndef DOSBOX_INITS_H_
#define DOSBOX_INITS_H_

void MSG_Init(Section_prop *);
void LOG_StartUp(void);
void MEM_Init(Section *);
void PAGING_Init(Section *);
void IO_Init(Section * );
void CALLBACK_Init(Section*);
void PROGRAMS_Init(Section*);
//void CREDITS_Init(Section*);
void RENDER_Init(Section*);
void VGA_Init(Section*);

void DOS_Init(Section*);


void CPU_Init(Section*);

#if C_FPU
void FPU_Init(Section*);
#endif

void DMA_Init(Section*);

void MIXER_Init(Section*);
void MIDI_Init(Section*);
void HARDWARE_Init(Section*);

#if defined(PCI_FUNCTIONALITY_ENABLED)
void PCI_Init(Section*);
#endif


void KEYBOARD_Init(Section*);	//TODO This should setup INT 16 too but ok ;)
void JOYSTICK_Init(Section*);
void MOUSE_Init(Section*);
void SBLASTER_Init(Section*);
void GUS_Init(Section*);
//void MPU401_Init(Section*);
void PCSPEAKER_Init(Section*);
//void TANDYSOUND_Init(Section*);
//void DISNEY_Init(Section*);
void SERIAL_Init(Section*); 


#if C_IPX
void IPX_Init(Section*);
#endif

void SID_Init(Section* sec);

void PIC_Init(Section*);
void TIMER_Init(Section*);
void BIOS_Init(Section*);
void DEBUG_Init(Section*);
void CMOS_Init(Section*);

void MSCDEX_Init(Section*);
void DRIVES_Init(Section*);
void CDROM_Image_Init(Section*);

/* Dos Internal mostly */
void EMS_Init(Section*);
void XMS_Init(Section*);

void DOS_KeyboardLayout_Init(Section*);

void AUTOEXEC_Init(Section*);
void SHELL_Init(void);

void INT10_Init(Section*);

#endif