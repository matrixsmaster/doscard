APP	:= dosbox
MNDIR	:= $(shell pwd)
CC_DEFS := -DHAVE_CONFIG_H -D_GNU_SOURCE=1 -D_REENTRANT
CC_FLAG := -g -O2
CC_INCL := -I$(MNDIR)/../include -I. -I$(MNDIR)/../../libsdl/include -I$(MNDIR)/../../SDL_sound
CC_COMP := g++ $(CC_DEFS) $(CC_FLAG) $(CC_INCL) -finstrument-functions
LD_STAT := -lSDL -lSDL_sound
LD_BDYN := -lX11 -lpthread
## -lcurses
LD_LIBS := -L../../libsdl/build/.libs/ -L../../SDL_sound/.libs/ -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)

LCPU_SRC := callback.cpp cpu.cpp flags.cpp modrm.cpp core_full.cpp paging.cpp core_normal.cpp core_simple.cpp fpu.cpp
LDBG_SRC := debug.cpp debug_gui.cpp debug_disasm.cpp
LDOS_SRC := dos.cpp dos_devices.cpp dos_execute.cpp dos_files.cpp dos_ioctl.cpp dos_memory.cpp dos_misc.cpp dos_classes.cpp dos_programs.cpp 
LDOS_SRC += dos_tables.cpp drives.cpp drive_virtual.cpp drive_local.cpp drive_cache.cpp drive_fat.cpp drive_iso.cpp dos_mscdex.cpp 
LDOS_SRC += dos_keyboard_layout.cpp cdrom.cpp cdrom_image.cpp
LTTY_SRC := directserial.cpp libserial.cpp serialdummy.cpp serialport.cpp misc_util.cpp nullmodem.cpp
LHDW_SRC := adlib.cpp dma.cpp gameblaster.cpp hardware.cpp iohandler.cpp joystick.cpp keyboard.cpp memory.cpp mixer.cpp pcspeaker.cpp pci_bus.cpp 
LHDW_SRC += pic.cpp sblaster.cpp timer.cpp vga.cpp vga_attr.cpp vga_crtc.cpp vga_dac.cpp vga_draw.cpp vga_gfx.cpp vga_other.cpp vga_memory.cpp 
LHDW_SRC += vga_misc.cpp vga_seq.cpp vga_xga.cpp vga_s3.cpp vga_tseng.cpp vga_paradise.cpp cmos.cpp gus.cpp dbopl.cpp
LIRQ_SRC := mouse.cpp xms.cpp ems.cpp int10.cpp int10_char.cpp int10_memory.cpp int10_misc.cpp int10_modes.cpp int10_vesa.cpp int10_pal.cpp 
LIRQ_SRC += int10_put_pixel.cpp int10_video_state.cpp int10_vptable.cpp bios.cpp bios_disk.cpp bios_keyboard.cpp
LGUI_SRC := sdlmain.cpp sdl_mapper.cpp render.cpp render_scalers.cpp
LSHL_SRC := shell.cpp shell_batch.cpp shell_cmds.cpp shell_misc.cpp
LMSC_SRC := cross.cpp messages.cpp programs.cpp setup.cpp support.cpp

APP_OBJS := $(addprefix cpu/,$(LCPU_SRC:.cpp=.o))
APP_OBJS += $(addprefix debug/,$(LDBG_SRC:.cpp=.o))
APP_OBJS += $(addprefix dos/,$(LDOS_SRC:.cpp=.o))
APP_OBJS += $(addprefix hardware/serialport/,$(LTTY_SRC:.cpp=.o))
APP_OBJS += $(addprefix hardware/,$(LHDW_SRC:.cpp=.o))
APP_OBJS += $(addprefix ints/,$(LIRQ_SRC:.cpp=.o))
APP_OBJS += $(addprefix gui/,$(LGUI_SRC:.cpp=.o))
APP_OBJS += $(addprefix shell/,$(LSHL_SRC:.cpp=.o))
APP_OBJS += $(addprefix misc/,$(LMSC_SRC:.cpp=.o))


all:	$(APP_OBJS) $(APP).o
	g++ -g -c instrument.cpp -o instrument.o
	g++ $(CC_FLAG) -o $(APP) $(APP).o instrument.o $(APP_OBJS) $(LD_LIBS)
.PHONY:	all

%.o:	%.cpp
	$(CC_COMP) -c -o $@ $<

clean:
	find . -type f -name '*.a' | xargs -n1 rm -fv
	find . -type f -name '*.o' | xargs -n1 rm -fv
	rm -f dosbox
