APP	:= dosbox
MNDIR	:= $(shell pwd)
CC_DEFS := -DHAVE_CONFIG_H -D_GNU_SOURCE=1 -D_REENTRANT
CC_FLAG := -g -O2
CC_INCL := -I$(MNDIR)/../include -I. -I/usr/include/SDL
CC_COMP := g++ $(CC_DEFS) $(CC_FLAG) $(CC_INCL)
LD_LIBS := -lSDL_sound -lSDL -lpng -lz -lX11 -lcurses

LCPU_SRC := callback.cpp cpu.cpp flags.cpp modrm.cpp core_full.cpp paging.cpp core_normal.cpp core_simple.cpp core_prefetch.cpp
LFPU_SRC := fpu.cpp
LDBG_SRC := debug.cpp debug_gui.cpp debug_disasm.cpp
LDOS_SRC := dos.cpp dos_devices.cpp dos_execute.cpp dos_files.cpp dos_ioctl.cpp dos_memory.cpp dos_misc.cpp dos_classes.cpp dos_programs.cpp 
LDOS_SRC += dos_tables.cpp drives.cpp drive_virtual.cpp drive_local.cpp drive_cache.cpp drive_fat.cpp drive_iso.cpp dos_mscdex.cpp dos_keyboard_layout.cpp cdrom.cpp 
LDOS_SRC += cdrom_ioctl_win32.cpp cdrom_aspi_win32.cpp cdrom_ioctl_linux.cpp cdrom_image.cpp cdrom_ioctl_os2.cpp
LTTY_SRC := directserial.cpp libserial.cpp serialdummy.cpp serialport.cpp misc_util.cpp nullmodem.cpp
LHDW_SRC := adlib.cpp dma.cpp gameblaster.cpp hardware.cpp iohandler.cpp joystick.cpp keyboard.cpp memory.cpp mixer.cpp pcspeaker.cpp pci_bus.cpp 
LHDW_SRC += pic.cpp sblaster.cpp tandy_sound.cpp timer.cpp vga.cpp vga_attr.cpp vga_crtc.cpp vga_dac.cpp vga_draw.cpp vga_gfx.cpp vga_other.cpp vga_memory.cpp 
LHDW_SRC += vga_misc.cpp vga_seq.cpp vga_xga.cpp vga_s3.cpp vga_tseng.cpp vga_paradise.cpp cmos.cpp disney.cpp gus.cpp mpu401.cpp dbopl.cpp
LIRQ_SRC := mouse.cpp xms.cpp ems.cpp int10.cpp int10_char.cpp int10_memory.cpp int10_misc.cpp int10_modes.cpp int10_vesa.cpp int10_pal.cpp 
LIRQ_SRC += int10_put_pixel.cpp int10_video_state.cpp int10_vptable.cpp bios.cpp bios_disk.cpp bios_keyboard.cpp
LGUI_SRC := sdlmain.cpp sdl_mapper.cpp render.cpp render_scalers.cpp midi.cpp sdl_gui.cpp
LSHL_SRC := shell.cpp shell_batch.cpp shell_cmds.cpp shell_misc.cpp
LMSC_SRC := cross.cpp messages.cpp programs.cpp setup.cpp support.cpp

LCPU_LIB := cpu/libcpu.a
LFPU_LIB := fpu/libfpu.a
LDBG_LIB := debug/libdebug.a
LDOS_LIB := dos/libdos.a
LTTY_LIB := hardware/serialport/libserial.a
LHDW_LIB := hardware/libhardware.a
LIRQ_LIB := ints/libints.a
LGUI_LIB := gui/libgui.a
LSHL_LIB := shell/libshell.a
LMSC_LIB := misc/libmisc.a

APP_LIBS := $(LCPU_LIB) $(LDBG_LIB) $(LDOS_LIB) $(LFPU_LIB) $(LHDW_LIB) $(LGUI_LIB) $(LIRQ_LIB) $(LMSC_LIB) $(LSHL_LIB) $(LTTY_LIB)

LCPU_INS := $(addprefix cpu/,$(LCPU_SRC:.cpp=.o))
LFPU_INS := $(addprefix fpu/,$(LFPU_SRC:.cpp=.o))
LDBG_INS := $(addprefix debug/,$(LDBG_SRC:.cpp=.o))
LDOS_INS := $(addprefix dos/,$(LDOS_SRC:.cpp=.o))
LTTY_INS := $(addprefix hardware/serialport/,$(LTTY_SRC:.cpp=.o))
LHDW_INS := $(addprefix hardware/,$(LHDW_SRC:.cpp=.o))
LIRQ_INS := $(addprefix ints/,$(LIRQ_SRC:.cpp=.o))
LGUI_INS := $(addprefix gui/,$(LGUI_SRC:.cpp=.o))
LSHL_INS := $(addprefix shell/,$(LSHL_SRC:.cpp=.o))
LMSC_INS := $(addprefix misc/,$(LMSC_SRC:.cpp=.o))

##buildlib := rm -f $(1) ; ar cru $(1) $(2) ; ranlib $(1)

all:	$(APP)
.PHONY:	all

$(APP): lcpu lfpu ldebug ldos ltty lhdw lirq lgui lshell lmisc $(APP).o
	g++ $(CC_FLAG) -o $(APP) $(APP).o $(APP_LIBS) $(LD_LIBS)

##lcpu:	$(LCPU_INS)
##	$(call buildlib,LCPU_LIB,LCPU_INS)

lcpu:	$(LCPU_INS)
	rm -f $(LCPU_LIB) ; ar cru $(LCPU_LIB) $(LCPU_INS) ; ranlib $(LCPU_LIB)
lfpu:	$(LFPU_INS)
	rm -f $(LFPU_LIB) ; ar cru $(LFPU_LIB) $(LFPU_INS) ; ranlib $(LFPU_LIB)
ldebug:	$(LDBG_INS)
	rm -f $(LDBG_LIB) ; ar cru $(LDBG_LIB) $(LDBG_INS) ; ranlib $(LDBG_LIB)
ldos:	$(LDOS_INS)
	rm -f $(LDOS_LIB) ; ar cru $(LDOS_LIB) $(LDOS_INS) ; ranlib $(LDOS_LIB)
ltty:	$(LTTY_INS)
	rm -f $(LTTY_LIB) ; ar cru $(LTTY_LIB) $(LTTY_INS) ; ranlib $(LTTY_LIB)
lhdw:	$(LHDW_INS)
	rm -f $(LHDW_LIB) ; ar cru $(LHDW_LIB) $(LHDW_INS) ; ranlib $(LHDW_LIB)
lirq:	$(LIRQ_INS)
	rm -f $(LIRQ_LIB) ; ar cru $(LIRQ_LIB) $(LIRQ_INS) ; ranlib $(LIRQ_LIB)
lgui:	$(LGUI_INS)
	rm -f $(LGUI_LIB) ; ar cru $(LGUI_LIB) $(LGUI_INS) ; ranlib $(LGUI_LIB)
lshell: $(LSHL_INS)
	rm -f $(LSHL_LIB) ; ar cru $(LSHL_LIB) $(LSHL_INS) ; ranlib $(LSHL_LIB)
lmisc:	$(LMSC_INS)
	rm -f $(LMSC_LIB) ; ar cru $(LMSC_LIB) $(LMSC_INS) ; ranlib $(LMSC_LIB)

%.o:	%.cpp
	$(CC_COMP) -c -o $@ $<

clean:
	find . -type f -name '*.a' | xargs -n1 rm -fv
	find . -type f -name '*.o' | xargs -n1 rm -fv
	rm -f dosbox
