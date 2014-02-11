## DosCard Project DosBox Module Main MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

LIBNAME	:= libdosbox
MNDIR	:= $(shell pwd)
BLDINFO := -DBUILDATE="\"$(shell date)\"" -DCOMPILERNAME="\"$(shell gcc --version | head -n1)\""
BLDINFO += -DBUILDNUMBER="\"$(shell cat buildno)\""
CC_DEFS := $(BLDINFO) -DHAVE_CONFIG_H -D_GNU_SOURCE
CC_FLAG := -g -O2
CC_INCL := -I$(MNDIR)/../include -I. -I../.. -I$(MNDIR)/../../xshell
CC_COMP := g++ $(CC_DEFS) $(CC_FLAG) $(CC_INCL) -finstrument-functions

LCPU_SRC := callback.cpp cpu.cpp flags.cpp modrm.cpp core_full.cpp paging.cpp core_normal.cpp core_simple.cpp fpu.cpp
LDOS_SRC := dos.cpp dos_devices.cpp dos_execute.cpp dos_files.cpp dos_ioctl.cpp dos_memory.cpp dos_misc.cpp dos_classes.cpp dos_programs.cpp 
LDOS_SRC += dos_tables.cpp drives.cpp drive_virtual.cpp drive_local.cpp drive_cache.cpp drive_fat.cpp drive_iso.cpp dos_mscdex.cpp 
LDOS_SRC += dos_keyboard_layout.cpp cdrom.cpp cdrom_image.cpp
LTTY_SRC := directserial.cpp libserial.cpp serialdummy.cpp serialport.cpp nullmodem.cpp
LHDW_SRC := dma.cpp iohandler.cpp keyboard.cpp memory.cpp mixer.cpp pcspeaker.cpp dbopl.cpp adlib.cpp pci_bus.cpp
LHDW_SRC += pic.cpp sblaster.cpp timer.cpp vga.cpp vga_attr.cpp vga_crtc.cpp vga_dac.cpp vga_draw.cpp vga_gfx.cpp vga_other.cpp vga_memory.cpp 
LHDW_SRC += vga_misc.cpp vga_seq.cpp vga_xga.cpp vga_s3.cpp vga_tseng.cpp vga_paradise.cpp cmos.cpp
LIRQ_SRC := mouse.cpp xms.cpp ems.cpp int10.cpp int10_char.cpp int10_memory.cpp int10_misc.cpp int10_modes.cpp int10_vesa.cpp int10_pal.cpp 
LIRQ_SRC += int10_put_pixel.cpp int10_video_state.cpp int10_vptable.cpp bios.cpp bios_disk.cpp bios_keyboard.cpp
LSHL_SRC := shell.cpp shell_batch.cpp shell_cmds.cpp shell_misc.cpp
LMSC_SRC := cross.cpp messages.cpp programs.cpp setup.cpp support.cpp

LIB_OBJS := $(addprefix cpu/,$(LCPU_SRC:.cpp=.o))
LIB_OBJS += $(addprefix dos/,$(LDOS_SRC:.cpp=.o))
LIB_OBJS += $(addprefix hardware/serialport/,$(LTTY_SRC:.cpp=.o))
LIB_OBJS += $(addprefix hardware/,$(LHDW_SRC:.cpp=.o))
LIB_OBJS += $(addprefix ints/,$(LIRQ_SRC:.cpp=.o))
LIB_OBJS += $(addprefix shell/,$(LSHL_SRC:.cpp=.o))
LIB_OBJS += $(addprefix misc/,$(LMSC_SRC:.cpp=.o))
LIB_OBJS += render.o xsmain.o xsfio.o dosbox.o


all:	$(LIB_OBJS)
	g++ -g -c instrument.cpp -o instrument.o
	rm -f $(LIBNAME).a
	ar cru $(LIBNAME).a instrument.o $(LIB_OBJS)
	ranlib $(LIBNAME).a
	expr `cat buildno` + 1 > buildno

.PHONY:	all

%.o:	%.cpp
	$(CC_COMP) -c -o $@ $<

clean:
	find . -type f -name '*.a' | xargs -n1 rm -fv
	find . -type f -name '*.o' | xargs -n1 rm -fv
