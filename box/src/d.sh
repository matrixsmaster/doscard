#!/bin/sh

export BUILDEF="-DHAVE_CONFIG_H -I. -I`pwd`/../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -c"

#find . -type d -name 'build' | xargs -n1 rm -rfv
find . -type f -name '*.a' | xargs -n1 rm -rfv
find . -type f -name '*.o' | xargs -n1 rm -rfv
rm -f dosbox

[ "x$1" = "xclean" ] && exit 0

cd cpu
#mkdir build
echo CPU
g++ $BUILDEF -o callback.o callback.cpp
g++ $BUILDEF -o cpu.o cpu.cpp
g++ $BUILDEF -o flags.o flags.cpp
g++ $BUILDEF -o modrm.o modrm.cpp
g++ $BUILDEF -o core_full.o core_full.cpp
g++ $BUILDEF -o paging.o paging.cpp
g++ $BUILDEF -o core_normal.o core_normal.cpp
g++ $BUILDEF -o core_simple.o core_simple.cpp
g++ $BUILDEF -o core_prefetch.o core_prefetch.cpp
rm -f libcpu.a
ar cru libcpu.a callback.o cpu.o flags.o modrm.o core_full.o paging.o core_normal.o core_simple.o core_prefetch.o 
ranlib libcpu.a

cd ../fpu
#mkdir build
echo FPU
g++ $BUILDEF -o fpu.o fpu.cpp
rm -f libfpu.a
ar cru libfpu.a fpu.o 
ranlib libfpu.a

cd ../debug
#mkdir build
echo DBG
g++ $BUILDEF -o debug.o debug.cpp
g++ $BUILDEF -o debug_gui.o debug_gui.cpp
g++ $BUILDEF -o debug_disasm.o debug_disasm.cpp
rm -f libdebug.a
ar cru libdebug.a debug.o debug_gui.o debug_disasm.o
ranlib libdebug.a

cd ../dos
#mkdir build
echo DOS
g++ $BUILDEF -o dos.o dos.cpp
g++ $BUILDEF -o dos_devices.o dos_devices.cpp
g++ $BUILDEF -o dos_execute.o dos_execute.cpp
g++ $BUILDEF -o dos_files.o dos_files.cpp
g++ $BUILDEF -o dos_ioctl.o dos_ioctl.cpp
g++ $BUILDEF -o dos_memory.o dos_memory.cpp
g++ $BUILDEF -o dos_misc.o dos_misc.cpp
g++ $BUILDEF -o dos_classes.o dos_classes.cpp
g++ $BUILDEF -o dos_programs.o dos_programs.cpp
g++ $BUILDEF -o dos_tables.o dos_tables.cpp
g++ $BUILDEF -o drives.o drives.cpp
g++ $BUILDEF -o drive_virtual.o drive_virtual.cpp
g++ $BUILDEF -o drive_local.o drive_local.cpp
g++ $BUILDEF -o drive_cache.o drive_cache.cpp
g++ $BUILDEF -o drive_fat.o drive_fat.cpp
g++ $BUILDEF -o drive_iso.o drive_iso.cpp
g++ $BUILDEF -o dos_mscdex.o dos_mscdex.cpp
g++ $BUILDEF -o dos_keyboard_layout.o dos_keyboard_layout.cpp
g++ $BUILDEF -o cdrom.o cdrom.cpp
g++ $BUILDEF -o cdrom_ioctl_win32.o cdrom_ioctl_win32.cpp
g++ $BUILDEF -o cdrom_aspi_win32.o cdrom_aspi_win32.cpp
g++ $BUILDEF -o cdrom_ioctl_linux.o cdrom_ioctl_linux.cpp
g++ $BUILDEF -o cdrom_image.o cdrom_image.cpp
g++ $BUILDEF -o cdrom_ioctl_os2.o cdrom_ioctl_os2.cpp
rm -f libdos.a
ar cru libdos.a dos.o dos_devices.o dos_execute.o dos_files.o dos_ioctl.o dos_memory.o dos_misc.o dos_classes.o dos_programs.o dos_tables.o drives.o drive_virtual.o drive_local.o drive_cache.o drive_fat.o drive_iso.o dos_mscdex.o dos_keyboard_layout.o cdrom.o cdrom_ioctl_win32.o cdrom_aspi_win32.o cdrom_ioctl_linux.o cdrom_image.o cdrom_ioctl_os2.o 
ranlib libdos.a

cd ../hardware
#mkdir build
echo HDW
g++ $BUILDEF -o adlib.o adlib.cpp
g++ $BUILDEF -o dma.o dma.cpp
g++ $BUILDEF -o gameblaster.o gameblaster.cpp
g++ $BUILDEF -o hardware.o hardware.cpp
g++ $BUILDEF -o iohandler.o iohandler.cpp
g++ $BUILDEF -o joystick.o joystick.cpp
g++ $BUILDEF -o keyboard.o keyboard.cpp
g++ $BUILDEF -o memory.o memory.cpp
g++ $BUILDEF -o mixer.o mixer.cpp
g++ $BUILDEF -o pcspeaker.o pcspeaker.cpp
g++ $BUILDEF -o pci_bus.o pci_bus.cpp
g++ $BUILDEF -o pic.o pic.cpp
g++ $BUILDEF -o sblaster.o sblaster.cpp
g++ $BUILDEF -o tandy_sound.o tandy_sound.cpp
g++ $BUILDEF -o timer.o timer.cpp
g++ $BUILDEF -o vga.o vga.cpp
g++ $BUILDEF -o vga_attr.o vga_attr.cpp
g++ $BUILDEF -o vga_crtc.o vga_crtc.cpp
g++ $BUILDEF -o vga_dac.o vga_dac.cpp
g++ $BUILDEF -o vga_draw.o vga_draw.cpp
g++ $BUILDEF -o vga_gfx.o vga_gfx.cpp
g++ $BUILDEF -o vga_other.o vga_other.cpp
g++ $BUILDEF -o vga_memory.o vga_memory.cpp
g++ $BUILDEF -o vga_misc.o vga_misc.cpp
g++ $BUILDEF -o vga_seq.o vga_seq.cpp
g++ $BUILDEF -o vga_xga.o vga_xga.cpp
g++ $BUILDEF -o vga_s3.o vga_s3.cpp
g++ $BUILDEF -o vga_tseng.o vga_tseng.cpp
g++ $BUILDEF -o vga_paradise.o vga_paradise.cpp
g++ $BUILDEF -o cmos.o cmos.cpp
g++ $BUILDEF -o disney.o disney.cpp
g++ $BUILDEF -o gus.o gus.cpp
g++ $BUILDEF -o mpu401.o mpu401.cpp
g++ $BUILDEF -o dbopl.o dbopl.cpp
rm -f libhardware.a
ar cru libhardware.a adlib.o dma.o gameblaster.o hardware.o iohandler.o joystick.o keyboard.o memory.o mixer.o pcspeaker.o pci_bus.o pic.o sblaster.o tandy_sound.o timer.o vga.o vga_attr.o vga_crtc.o vga_dac.o vga_draw.o vga_gfx.o vga_other.o vga_memory.o vga_misc.o vga_seq.o vga_xga.o vga_s3.o vga_tseng.o vga_paradise.o cmos.o disney.o gus.o mpu401.o dbopl.o 
ranlib libhardware.a

cd serialport
#mkdir build
echo TTY
g++ $BUILDEF -o directserial.o directserial.cpp
g++ $BUILDEF -o libserial.o libserial.cpp
g++ $BUILDEF -o serialdummy.o serialdummy.cpp
g++ $BUILDEF -o serialport.o serialport.cpp
g++ $BUILDEF -o misc_util.o misc_util.cpp
g++ $BUILDEF -o nullmodem.o nullmodem.cpp
rm -f libserial.a
ar cru libserial.a directserial.o libserial.o serialdummy.o serialport.o misc_util.o nullmodem.o 
ranlib libserial.a

cd ../../ints
#mkdir build
echo IRQ
g++ $BUILDEF -o mouse.o mouse.cpp
g++ $BUILDEF -o xms.o xms.cpp
g++ $BUILDEF -o ems.o ems.cpp
g++ $BUILDEF -o int10.o int10.cpp
g++ $BUILDEF -o int10_char.o int10_char.cpp
g++ $BUILDEF -o int10_memory.o int10_memory.cpp
g++ $BUILDEF -o int10_misc.o int10_misc.cpp
g++ $BUILDEF -o int10_modes.o int10_modes.cpp
g++ $BUILDEF -o int10_vesa.o int10_vesa.cpp
g++ $BUILDEF -o int10_pal.o int10_pal.cpp
g++ $BUILDEF -o int10_put_pixel.o int10_put_pixel.cpp
g++ $BUILDEF -o int10_video_state.o int10_video_state.cpp
g++ $BUILDEF -o int10_vptable.o int10_vptable.cpp
g++ $BUILDEF -o bios.o bios.cpp
g++ $BUILDEF -o bios_disk.o bios_disk.cpp
g++ $BUILDEF -o bios_keyboard.o bios_keyboard.cpp
rm -f libints.a
ar cru libints.a mouse.o xms.o ems.o int10.o int10_char.o int10_memory.o int10_misc.o int10_modes.o int10_vesa.o int10_pal.o int10_put_pixel.o int10_video_state.o int10_vptable.o bios.o bios_disk.o bios_keyboard.o 
ranlib libints.a


cd ../misc
#mkdir build
echo MSC
g++ $BUILDEF -o cross.o cross.cpp
g++ $BUILDEF -o messages.o messages.cpp
g++ $BUILDEF -o programs.o programs.cpp
g++ $BUILDEF -o setup.o setup.cpp
g++ $BUILDEF -o support.o support.cpp
rm -f libmisc.a
ar cru libmisc.a cross.o messages.o programs.o setup.o support.o 
ranlib libmisc.a

cd ../shell
#mkdir build
echo SHL
g++ $BUILDEF -o shell.o shell.cpp
g++ $BUILDEF -o shell_batch.o shell_batch.cpp
g++ $BUILDEF -o shell_cmds.o shell_cmds.cpp
g++ $BUILDEF -o shell_misc.o shell_misc.cpp
rm -f libshell.a
ar cru libshell.a shell.o shell_batch.o shell_cmds.o shell_misc.o 
ranlib libshell.a

cd ../gui
#mkdir build
echo GUI
g++ $BUILDEF -o sdlmain.o sdlmain.cpp
g++ $BUILDEF -o sdl_mapper.o sdl_mapper.cpp
g++ $BUILDEF -o render.o render.cpp
g++ $BUILDEF -o render_scalers.o render_scalers.cpp
g++ $BUILDEF -o midi.o midi.cpp
g++ $BUILDEF -o sdl_gui.o sdl_gui.cpp
rm -f libgui.a
ar cru libgui.a sdlmain.o sdl_mapper.o render.o render_scalers.o midi.o sdl_gui.o 
ranlib libgui.a

cd ..
#mkdir build
echo MAIN
g++ $BUILDEF -o dosbox.o dosbox.cpp
g++  -g -O2   -o dosbox dosbox.o  cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a -lSDL_sound  -L/usr/lib -lSDL -lpng -lz -lX11 -lcurses
ls
