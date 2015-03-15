## DosCard Project DosBox Module Main MAKEFILE
## Copyright (C) 2013-2014 Dmitry Soloviov

## Internal name. Do not use 'libdosbox' in external projects as it may interfere with
## some existing code and/or libraries.
LIBNAME	:= libdosbox
CXX		:= g++-4.8.3
MNDIR	:= $(shell pwd)
BLDINFO := -DBUILDATE="\"$(shell date)\""
BLDINFO += -DCOMPILERNAME="\"$(shell $(CXX) --version | head -n1)\""
BLDINFO += -DBUILDNUMBER="\"$(shell cat buildno)\""
CC_DEFS := $(BLDINFO) -DHAVE_CONFIG_H -D_GNU_SOURCE -DHOST_64BIT
CC_FLAG := -g -O2 -Wall -Wextra -finstrument-functions -fno-strict-aliasing $(EXT_CC_FLAGS)

CC_INCL := -I$(MNDIR)/../include -I. -I../..
CC_COMP := $(CXX) $(CC_DEFS) $(CC_FLAG) $(CC_INCL)

LCPU_SRC := callback.cpp cpu.cpp flags.cpp modrm.cpp core_full.cpp paging.cpp
LCPU_SRC += core_normal.cpp core_simple.cpp fpu.cpp

LDOS_SRC := dos.cpp dos_devices.cpp dos_execute.cpp dos_files.cpp dos_ioctl.cpp dos_memory.cpp
LDOS_SRC += dos_misc.cpp dos_classes.cpp dos_programs.cpp dos_tables.cpp drives.cpp drive_virtual.cpp
LDOS_SRC += drive_local.cpp drive_cache.cpp drive_fat.cpp drive_iso.cpp dos_mscdex.cpp 
LDOS_SRC += dos_keyboard_layout.cpp cdrom.cpp cdrom_image.cpp

LHDW_SRC := dma.cpp iohandler.cpp keyboard.cpp memory.cpp mixer.cpp pcspeaker.cpp dbopl.cpp
LHDW_SRC += adlib.cpp pci_bus.cpp pic.cpp timer.cpp sblaster.cpp vga.cpp vga_attr.cpp vga_crtc.cpp
LHDW_SRC += vga_dac.cpp vga_draw.cpp vga_gfx.cpp vga_other.cpp vga_memory.cpp vga_misc.cpp
LHDW_SRC += vga_seq.cpp vga_xga.cpp vga_s3.cpp cmos.cpp serialport.cpp serialdummy.cpp

LIRQ_SRC := mouse.cpp xms.cpp ems.cpp int10.cpp int10_char.cpp int10_memory.cpp int10_misc.cpp
LIRQ_SRC += int10_modes.cpp int10_vesa.cpp int10_pal.cpp int10_put_pixel.cpp int10_video_state.cpp
LIRQ_SRC += int10_vptable.cpp bios.cpp bios_disk.cpp bios_keyboard.cpp

LSHL_SRC := shell.cpp shell_batch.cpp shell_cmds.cpp shell_misc.cpp

LMSC_SRC := programs.cpp support.cpp ldbconf.cpp

LTOP_SRC := render.cpp xsgfx.cpp xsfio.cpp fake_threading.cpp dosbox.cpp


LIB_OBJS := $(addprefix cpu/,$(LCPU_SRC:.cpp=.o))
LIB_OBJS += $(addprefix dos/,$(LDOS_SRC:.cpp=.o))
LIB_OBJS += $(addprefix hardware/,$(LHDW_SRC:.cpp=.o))
LIB_OBJS += $(addprefix ints/,$(LIRQ_SRC:.cpp=.o))
LIB_OBJS += $(addprefix shell/,$(LSHL_SRC:.cpp=.o))
LIB_OBJS += $(addprefix misc/,$(LMSC_SRC:.cpp=.o))
LIB_OBJS += $(LTOP_SRC:.cpp=.o)


all:	$(LIB_OBJS)
##TODO: make instrumentation compilation conditional 
	$(CXX) -g -c $(EXT_CC_FLAGS) instrument.cpp -o instrument.o
	rm -f $(LIBNAME).a
	ar cru $(LIBNAME).a instrument.o $(LIB_OBJS)
	echo $(LIBNAME).a instrument.o $(LIB_OBJS) 1> objs.txt
	ranlib $(LIBNAME).a
	expr `cat buildno` + 1 > buildno

.PHONY:	all

%.o:	%.cpp
	$(CC_COMP) -c -o $@ $<

clean:
	rm -f objs.txt
	find . -type f -name '*.a' | xargs -n1 rm -fv
	find . -type f -name '*.o' | xargs -n1 rm -fv
	find . -type f -name '*.bc' | xargs -n1 rm -fv
