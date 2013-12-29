#!/bin/sh

find . -type d -name '.deps' | xargs -n1 rm -rfv
rm -f dosbox

cd cpu
mkdir .deps
echo CPU
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT callback.o -MD -MP -MF .deps/callback.Tpo -c -o callback.o callback.cpp
mv -f .deps/callback.Tpo .deps/callback.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cpu.o -MD -MP -MF .deps/cpu.Tpo -c -o cpu.o cpu.cpp
mv -f .deps/cpu.Tpo .deps/cpu.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT flags.o -MD -MP -MF .deps/flags.Tpo -c -o flags.o flags.cpp
mv -f .deps/flags.Tpo .deps/flags.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT modrm.o -MD -MP -MF .deps/modrm.Tpo -c -o modrm.o modrm.cpp
mv -f .deps/modrm.Tpo .deps/modrm.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_full.o -MD -MP -MF .deps/core_full.Tpo -c -o core_full.o core_full.cpp
mv -f .deps/core_full.Tpo .deps/core_full.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT paging.o -MD -MP -MF .deps/paging.Tpo -c -o paging.o paging.cpp
mv -f .deps/paging.Tpo .deps/paging.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_normal.o -MD -MP -MF .deps/core_normal.Tpo -c -o core_normal.o core_normal.cpp
mv -f .deps/core_normal.Tpo .deps/core_normal.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_simple.o -MD -MP -MF .deps/core_simple.Tpo -c -o core_simple.o core_simple.cpp
mv -f .deps/core_simple.Tpo .deps/core_simple.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_prefetch.o -MD -MP -MF .deps/core_prefetch.Tpo -c -o core_prefetch.o core_prefetch.cpp
mv -f .deps/core_prefetch.Tpo .deps/core_prefetch.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_dyn_x86.o -MD -MP -MF .deps/core_dyn_x86.Tpo -c -o core_dyn_x86.o core_dyn_x86.cpp
mv -f .deps/core_dyn_x86.Tpo .deps/core_dyn_x86.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_dynrec.o -MD -MP -MF .deps/core_dynrec.Tpo -c -o core_dynrec.o core_dynrec.cpp
mv -f .deps/core_dynrec.Tpo .deps/core_dynrec.Po
rm -f libcpu.a
ar cru libcpu.a callback.o cpu.o flags.o modrm.o core_full.o paging.o core_normal.o core_simple.o core_prefetch.o core_dyn_x86.o core_dynrec.o 
ranlib libcpu.a

cd ../fpu
mkdir .deps
echo FPU
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT fpu.o -MD -MP -MF .deps/fpu.Tpo -c -o fpu.o fpu.cpp
mv -f .deps/fpu.Tpo .deps/fpu.Po
rm -f libfpu.a
ar cru libfpu.a fpu.o 
ranlib libfpu.a

cd ../debug
mkdir .deps
echo DBG
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug.o -MD -MP -MF .deps/debug.Tpo -c -o debug.o debug.cpp
mv -f .deps/debug.Tpo .deps/debug.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_gui.o -MD -MP -MF .deps/debug_gui.Tpo -c -o debug_gui.o debug_gui.cpp
mv -f .deps/debug_gui.Tpo .deps/debug_gui.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_disasm.o -MD -MP -MF .deps/debug_disasm.Tpo -c -o debug_disasm.o debug_disasm.cpp
mv -f .deps/debug_disasm.Tpo .deps/debug_disasm.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_win32.o -MD -MP -MF .deps/debug_win32.Tpo -c -o debug_win32.o debug_win32.cpp
mv -f .deps/debug_win32.Tpo .deps/debug_win32.Po
rm -f libdebug.a
ar cru libdebug.a debug.o debug_gui.o debug_disasm.o debug_win32.o 
ranlib libdebug.a

cd ../dos
mkdir .deps
echo DOS
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos.o -MD -MP -MF .deps/dos.Tpo -c -o dos.o dos.cpp
mv -f .deps/dos.Tpo .deps/dos.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_devices.o -MD -MP -MF .deps/dos_devices.Tpo -c -o dos_devices.o dos_devices.cpp
mv -f .deps/dos_devices.Tpo .deps/dos_devices.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_execute.o -MD -MP -MF .deps/dos_execute.Tpo -c -o dos_execute.o dos_execute.cpp
mv -f .deps/dos_execute.Tpo .deps/dos_execute.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_files.o -MD -MP -MF .deps/dos_files.Tpo -c -o dos_files.o dos_files.cpp
mv -f .deps/dos_files.Tpo .deps/dos_files.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_ioctl.o -MD -MP -MF .deps/dos_ioctl.Tpo -c -o dos_ioctl.o dos_ioctl.cpp
mv -f .deps/dos_ioctl.Tpo .deps/dos_ioctl.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_memory.o -MD -MP -MF .deps/dos_memory.Tpo -c -o dos_memory.o dos_memory.cpp
mv -f .deps/dos_memory.Tpo .deps/dos_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_misc.o -MD -MP -MF .deps/dos_misc.Tpo -c -o dos_misc.o dos_misc.cpp
mv -f .deps/dos_misc.Tpo .deps/dos_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_classes.o -MD -MP -MF .deps/dos_classes.Tpo -c -o dos_classes.o dos_classes.cpp
mv -f .deps/dos_classes.Tpo .deps/dos_classes.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_programs.o -MD -MP -MF .deps/dos_programs.Tpo -c -o dos_programs.o dos_programs.cpp
mv -f .deps/dos_programs.Tpo .deps/dos_programs.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_tables.o -MD -MP -MF .deps/dos_tables.Tpo -c -o dos_tables.o dos_tables.cpp
mv -f .deps/dos_tables.Tpo .deps/dos_tables.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drives.o -MD -MP -MF .deps/drives.Tpo -c -o drives.o drives.cpp
mv -f .deps/drives.Tpo .deps/drives.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_virtual.o -MD -MP -MF .deps/drive_virtual.Tpo -c -o drive_virtual.o drive_virtual.cpp
mv -f .deps/drive_virtual.Tpo .deps/drive_virtual.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_local.o -MD -MP -MF .deps/drive_local.Tpo -c -o drive_local.o drive_local.cpp
mv -f .deps/drive_local.Tpo .deps/drive_local.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_cache.o -MD -MP -MF .deps/drive_cache.Tpo -c -o drive_cache.o drive_cache.cpp
mv -f .deps/drive_cache.Tpo .deps/drive_cache.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_fat.o -MD -MP -MF .deps/drive_fat.Tpo -c -o drive_fat.o drive_fat.cpp
mv -f .deps/drive_fat.Tpo .deps/drive_fat.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_iso.o -MD -MP -MF .deps/drive_iso.Tpo -c -o drive_iso.o drive_iso.cpp
mv -f .deps/drive_iso.Tpo .deps/drive_iso.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_mscdex.o -MD -MP -MF .deps/dos_mscdex.Tpo -c -o dos_mscdex.o dos_mscdex.cpp
mv -f .deps/dos_mscdex.Tpo .deps/dos_mscdex.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_keyboard_layout.o -MD -MP -MF .deps/dos_keyboard_layout.Tpo -c -o dos_keyboard_layout.o dos_keyboard_layout.cpp
mv -f .deps/dos_keyboard_layout.Tpo .deps/dos_keyboard_layout.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom.o -MD -MP -MF .deps/cdrom.Tpo -c -o cdrom.o cdrom.cpp
mv -f .deps/cdrom.Tpo .deps/cdrom.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_win32.o -MD -MP -MF .deps/cdrom_ioctl_win32.Tpo -c -o cdrom_ioctl_win32.o cdrom_ioctl_win32.cpp
mv -f .deps/cdrom_ioctl_win32.Tpo .deps/cdrom_ioctl_win32.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_aspi_win32.o -MD -MP -MF .deps/cdrom_aspi_win32.Tpo -c -o cdrom_aspi_win32.o cdrom_aspi_win32.cpp
mv -f .deps/cdrom_aspi_win32.Tpo .deps/cdrom_aspi_win32.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_linux.o -MD -MP -MF .deps/cdrom_ioctl_linux.Tpo -c -o cdrom_ioctl_linux.o cdrom_ioctl_linux.cpp
mv -f .deps/cdrom_ioctl_linux.Tpo .deps/cdrom_ioctl_linux.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_image.o -MD -MP -MF .deps/cdrom_image.Tpo -c -o cdrom_image.o cdrom_image.cpp
mv -f .deps/cdrom_image.Tpo .deps/cdrom_image.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_os2.o -MD -MP -MF .deps/cdrom_ioctl_os2.Tpo -c -o cdrom_ioctl_os2.o cdrom_ioctl_os2.cpp
mv -f .deps/cdrom_ioctl_os2.Tpo .deps/cdrom_ioctl_os2.Po
rm -f libdos.a
ar cru libdos.a dos.o dos_devices.o dos_execute.o dos_files.o dos_ioctl.o dos_memory.o dos_misc.o dos_classes.o dos_programs.o dos_tables.o drives.o drive_virtual.o drive_local.o drive_cache.o drive_fat.o drive_iso.o dos_mscdex.o dos_keyboard_layout.o cdrom.o cdrom_ioctl_win32.o cdrom_aspi_win32.o cdrom_ioctl_linux.o cdrom_image.o cdrom_ioctl_os2.o 
ranlib libdos.a

cd ../hardware
mkdir .deps
echo HDW
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT adlib.o -MD -MP -MF .deps/adlib.Tpo -c -o adlib.o adlib.cpp
mv -f .deps/adlib.Tpo .deps/adlib.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dma.o -MD -MP -MF .deps/dma.Tpo -c -o dma.o dma.cpp
mv -f .deps/dma.Tpo .deps/dma.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gameblaster.o -MD -MP -MF .deps/gameblaster.Tpo -c -o gameblaster.o gameblaster.cpp
mv -f .deps/gameblaster.Tpo .deps/gameblaster.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT hardware.o -MD -MP -MF .deps/hardware.Tpo -c -o hardware.o hardware.cpp
mv -f .deps/hardware.Tpo .deps/hardware.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT iohandler.o -MD -MP -MF .deps/iohandler.Tpo -c -o iohandler.o iohandler.cpp
mv -f .deps/iohandler.Tpo .deps/iohandler.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT joystick.o -MD -MP -MF .deps/joystick.Tpo -c -o joystick.o joystick.cpp
mv -f .deps/joystick.Tpo .deps/joystick.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT keyboard.o -MD -MP -MF .deps/keyboard.Tpo -c -o keyboard.o keyboard.cpp
mv -f .deps/keyboard.Tpo .deps/keyboard.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT memory.o -MD -MP -MF .deps/memory.Tpo -c -o memory.o memory.cpp
mv -f .deps/memory.Tpo .deps/memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mixer.o -MD -MP -MF .deps/mixer.Tpo -c -o mixer.o mixer.cpp
mv -f .deps/mixer.Tpo .deps/mixer.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pcspeaker.o -MD -MP -MF .deps/pcspeaker.Tpo -c -o pcspeaker.o pcspeaker.cpp
mv -f .deps/pcspeaker.Tpo .deps/pcspeaker.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pci_bus.o -MD -MP -MF .deps/pci_bus.Tpo -c -o pci_bus.o pci_bus.cpp
mv -f .deps/pci_bus.Tpo .deps/pci_bus.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pic.o -MD -MP -MF .deps/pic.Tpo -c -o pic.o pic.cpp
mv -f .deps/pic.Tpo .deps/pic.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sblaster.o -MD -MP -MF .deps/sblaster.Tpo -c -o sblaster.o sblaster.cpp
mv -f .deps/sblaster.Tpo .deps/sblaster.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT tandy_sound.o -MD -MP -MF .deps/tandy_sound.Tpo -c -o tandy_sound.o tandy_sound.cpp
mv -f .deps/tandy_sound.Tpo .deps/tandy_sound.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT timer.o -MD -MP -MF .deps/timer.Tpo -c -o timer.o timer.cpp
mv -f .deps/timer.Tpo .deps/timer.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga.o -MD -MP -MF .deps/vga.Tpo -c -o vga.o vga.cpp
mv -f .deps/vga.Tpo .deps/vga.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_attr.o -MD -MP -MF .deps/vga_attr.Tpo -c -o vga_attr.o vga_attr.cpp
mv -f .deps/vga_attr.Tpo .deps/vga_attr.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_crtc.o -MD -MP -MF .deps/vga_crtc.Tpo -c -o vga_crtc.o vga_crtc.cpp
mv -f .deps/vga_crtc.Tpo .deps/vga_crtc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_dac.o -MD -MP -MF .deps/vga_dac.Tpo -c -o vga_dac.o vga_dac.cpp
mv -f .deps/vga_dac.Tpo .deps/vga_dac.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_draw.o -MD -MP -MF .deps/vga_draw.Tpo -c -o vga_draw.o vga_draw.cpp
mv -f .deps/vga_draw.Tpo .deps/vga_draw.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_gfx.o -MD -MP -MF .deps/vga_gfx.Tpo -c -o vga_gfx.o vga_gfx.cpp
mv -f .deps/vga_gfx.Tpo .deps/vga_gfx.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_other.o -MD -MP -MF .deps/vga_other.Tpo -c -o vga_other.o vga_other.cpp
mv -f .deps/vga_other.Tpo .deps/vga_other.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_memory.o -MD -MP -MF .deps/vga_memory.Tpo -c -o vga_memory.o vga_memory.cpp
mv -f .deps/vga_memory.Tpo .deps/vga_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_misc.o -MD -MP -MF .deps/vga_misc.Tpo -c -o vga_misc.o vga_misc.cpp
mv -f .deps/vga_misc.Tpo .deps/vga_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_seq.o -MD -MP -MF .deps/vga_seq.Tpo -c -o vga_seq.o vga_seq.cpp
mv -f .deps/vga_seq.Tpo .deps/vga_seq.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_xga.o -MD -MP -MF .deps/vga_xga.Tpo -c -o vga_xga.o vga_xga.cpp
mv -f .deps/vga_xga.Tpo .deps/vga_xga.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_s3.o -MD -MP -MF .deps/vga_s3.Tpo -c -o vga_s3.o vga_s3.cpp
mv -f .deps/vga_s3.Tpo .deps/vga_s3.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_tseng.o -MD -MP -MF .deps/vga_tseng.Tpo -c -o vga_tseng.o vga_tseng.cpp
mv -f .deps/vga_tseng.Tpo .deps/vga_tseng.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_paradise.o -MD -MP -MF .deps/vga_paradise.Tpo -c -o vga_paradise.o vga_paradise.cpp
mv -f .deps/vga_paradise.Tpo .deps/vga_paradise.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cmos.o -MD -MP -MF .deps/cmos.Tpo -c -o cmos.o cmos.cpp
mv -f .deps/cmos.Tpo .deps/cmos.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT disney.o -MD -MP -MF .deps/disney.Tpo -c -o disney.o disney.cpp
mv -f .deps/disney.Tpo .deps/disney.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gus.o -MD -MP -MF .deps/gus.Tpo -c -o gus.o gus.cpp
mv -f .deps/gus.Tpo .deps/gus.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mpu401.o -MD -MP -MF .deps/mpu401.Tpo -c -o mpu401.o mpu401.cpp
mv -f .deps/mpu401.Tpo .deps/mpu401.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ipx.o -MD -MP -MF .deps/ipx.Tpo -c -o ipx.o ipx.cpp
mv -f .deps/ipx.Tpo .deps/ipx.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ipxserver.o -MD -MP -MF .deps/ipxserver.Tpo -c -o ipxserver.o ipxserver.cpp
mv -f .deps/ipxserver.Tpo .deps/ipxserver.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dbopl.o -MD -MP -MF .deps/dbopl.Tpo -c -o dbopl.o dbopl.cpp
mv -f .deps/dbopl.Tpo .deps/dbopl.Po
rm -f libhardware.a
ar cru libhardware.a adlib.o dma.o gameblaster.o hardware.o iohandler.o joystick.o keyboard.o memory.o mixer.o pcspeaker.o pci_bus.o pic.o sblaster.o tandy_sound.o timer.o vga.o vga_attr.o vga_crtc.o vga_dac.o vga_draw.o vga_gfx.o vga_other.o vga_memory.o vga_misc.o vga_seq.o vga_xga.o vga_s3.o vga_tseng.o vga_paradise.o cmos.o disney.o gus.o mpu401.o ipx.o ipxserver.o dbopl.o 
ranlib libhardware.a

cd serialport
mkdir .deps
echo TTY
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT directserial.o -MD -MP -MF .deps/directserial.Tpo -c -o directserial.o directserial.cpp
mv -f .deps/directserial.Tpo .deps/directserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT libserial.o -MD -MP -MF .deps/libserial.Tpo -c -o libserial.o libserial.cpp
mv -f .deps/libserial.Tpo .deps/libserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialdummy.o -MD -MP -MF .deps/serialdummy.Tpo -c -o serialdummy.o serialdummy.cpp
mv -f .deps/serialdummy.Tpo .deps/serialdummy.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialport.o -MD -MP -MF .deps/serialport.Tpo -c -o serialport.o serialport.cpp
mv -f .deps/serialport.Tpo .deps/serialport.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT softmodem.o -MD -MP -MF .deps/softmodem.Tpo -c -o softmodem.o softmodem.cpp
mv -f .deps/softmodem.Tpo .deps/softmodem.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT misc_util.o -MD -MP -MF .deps/misc_util.Tpo -c -o misc_util.o misc_util.cpp
mv -f .deps/misc_util.Tpo .deps/misc_util.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT nullmodem.o -MD -MP -MF .deps/nullmodem.Tpo -c -o nullmodem.o nullmodem.cpp
mv -f .deps/nullmodem.Tpo .deps/nullmodem.Po
rm -f libserial.a
ar cru libserial.a directserial.o libserial.o serialdummy.o serialport.o softmodem.o misc_util.o nullmodem.o 
ranlib libserial.a

cd ../../ints
mkdir .deps
echo IRQ
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mouse.o -MD -MP -MF .deps/mouse.Tpo -c -o mouse.o mouse.cpp
mv -f .deps/mouse.Tpo .deps/mouse.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT xms.o -MD -MP -MF .deps/xms.Tpo -c -o xms.o xms.cpp
mv -f .deps/xms.Tpo .deps/xms.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ems.o -MD -MP -MF .deps/ems.Tpo -c -o ems.o ems.cpp
mv -f .deps/ems.Tpo .deps/ems.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10.o -MD -MP -MF .deps/int10.Tpo -c -o int10.o int10.cpp
mv -f .deps/int10.Tpo .deps/int10.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_char.o -MD -MP -MF .deps/int10_char.Tpo -c -o int10_char.o int10_char.cpp
mv -f .deps/int10_char.Tpo .deps/int10_char.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_memory.o -MD -MP -MF .deps/int10_memory.Tpo -c -o int10_memory.o int10_memory.cpp
mv -f .deps/int10_memory.Tpo .deps/int10_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_misc.o -MD -MP -MF .deps/int10_misc.Tpo -c -o int10_misc.o int10_misc.cpp
mv -f .deps/int10_misc.Tpo .deps/int10_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_modes.o -MD -MP -MF .deps/int10_modes.Tpo -c -o int10_modes.o int10_modes.cpp
mv -f .deps/int10_modes.Tpo .deps/int10_modes.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_vesa.o -MD -MP -MF .deps/int10_vesa.Tpo -c -o int10_vesa.o int10_vesa.cpp
mv -f .deps/int10_vesa.Tpo .deps/int10_vesa.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_pal.o -MD -MP -MF .deps/int10_pal.Tpo -c -o int10_pal.o int10_pal.cpp
mv -f .deps/int10_pal.Tpo .deps/int10_pal.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_put_pixel.o -MD -MP -MF .deps/int10_put_pixel.Tpo -c -o int10_put_pixel.o int10_put_pixel.cpp
mv -f .deps/int10_put_pixel.Tpo .deps/int10_put_pixel.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_video_state.o -MD -MP -MF .deps/int10_video_state.Tpo -c -o int10_video_state.o int10_video_state.cpp
mv -f .deps/int10_video_state.Tpo .deps/int10_video_state.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_vptable.o -MD -MP -MF .deps/int10_vptable.Tpo -c -o int10_vptable.o int10_vptable.cpp
mv -f .deps/int10_vptable.Tpo .deps/int10_vptable.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios.o -MD -MP -MF .deps/bios.Tpo -c -o bios.o bios.cpp
mv -f .deps/bios.Tpo .deps/bios.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios_disk.o -MD -MP -MF .deps/bios_disk.Tpo -c -o bios_disk.o bios_disk.cpp
mv -f .deps/bios_disk.Tpo .deps/bios_disk.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios_keyboard.o -MD -MP -MF .deps/bios_keyboard.Tpo -c -o bios_keyboard.o bios_keyboard.cpp
mv -f .deps/bios_keyboard.Tpo .deps/bios_keyboard.Po
rm -f libints.a
ar cru libints.a mouse.o xms.o ems.o int10.o int10_char.o int10_memory.o int10_misc.o int10_modes.o int10_vesa.o int10_pal.o int10_put_pixel.o int10_video_state.o int10_vptable.o bios.o bios_disk.o bios_keyboard.o 
ranlib libints.a

cd ../libs/gui_tk
mkdir .deps
echo GTK
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gui_tk.o -MD -MP -MF .deps/gui_tk.Tpo -c -o gui_tk.o gui_tk.cpp
mv -f .deps/gui_tk.Tpo .deps/gui_tk.Po
rm -f libgui_tk.a
ar cru libgui_tk.a gui_tk.o 
ranlib libgui_tk.a

cd ../../misc
mkdir .deps
echo MSC
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cross.o -MD -MP -MF .deps/cross.Tpo -c -o cross.o cross.cpp
mv -f .deps/cross.Tpo .deps/cross.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT messages.o -MD -MP -MF .deps/messages.Tpo -c -o messages.o messages.cpp
mv -f .deps/messages.Tpo .deps/messages.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT programs.o -MD -MP -MF .deps/programs.Tpo -c -o programs.o programs.cpp
mv -f .deps/programs.Tpo .deps/programs.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT setup.o -MD -MP -MF .deps/setup.Tpo -c -o setup.o setup.cpp
mv -f .deps/setup.Tpo .deps/setup.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT support.o -MD -MP -MF .deps/support.Tpo -c -o support.o support.cpp
mv -f .deps/support.Tpo .deps/support.Po
rm -f libmisc.a
ar cru libmisc.a cross.o messages.o programs.o setup.o support.o 
ranlib libmisc.a

cd ../shell
mkdir .deps
echo SHL
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell.o -MD -MP -MF .deps/shell.Tpo -c -o shell.o shell.cpp
mv -f .deps/shell.Tpo .deps/shell.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_batch.o -MD -MP -MF .deps/shell_batch.Tpo -c -o shell_batch.o shell_batch.cpp
mv -f .deps/shell_batch.Tpo .deps/shell_batch.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_cmds.o -MD -MP -MF .deps/shell_cmds.Tpo -c -o shell_cmds.o shell_cmds.cpp
mv -f .deps/shell_cmds.Tpo .deps/shell_cmds.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_misc.o -MD -MP -MF .deps/shell_misc.Tpo -c -o shell_misc.o shell_misc.cpp
mv -f .deps/shell_misc.Tpo .deps/shell_misc.Po
rm -f libshell.a
ar cru libshell.a shell.o shell_batch.o shell_cmds.o shell_misc.o 
ranlib libshell.a

cd ../gui
mkdir .deps
echo GUI
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdlmain.o -MD -MP -MF .deps/sdlmain.Tpo -c -o sdlmain.o sdlmain.cpp
mv -f .deps/sdlmain.Tpo .deps/sdlmain.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_mapper.o -MD -MP -MF .deps/sdl_mapper.Tpo -c -o sdl_mapper.o sdl_mapper.cpp
mv -f .deps/sdl_mapper.Tpo .deps/sdl_mapper.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render.o -MD -MP -MF .deps/render.Tpo -c -o render.o render.cpp
mv -f .deps/render.Tpo .deps/render.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render_scalers.o -MD -MP -MF .deps/render_scalers.Tpo -c -o render_scalers.o render_scalers.cpp
mv -f .deps/render_scalers.Tpo .deps/render_scalers.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT midi.o -MD -MP -MF .deps/midi.Tpo -c -o midi.o midi.cpp
mv -f .deps/midi.Tpo .deps/midi.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_gui.o -MD -MP -MF .deps/sdl_gui.Tpo -c -o sdl_gui.o sdl_gui.cpp
mv -f .deps/sdl_gui.Tpo .deps/sdl_gui.Po
rm -f libgui.a
ar cru libgui.a sdlmain.o sdl_mapper.o render.o render_scalers.o midi.o sdl_gui.o 
ranlib libgui.a

cd ..
mkdir .deps
echo MAIN
g++ -DHAVE_CONFIG_H -I. -I..  -I../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dosbox.o -MD -MP -MF .deps/dosbox.Tpo -c -o dosbox.o dosbox.cpp
mv -f .deps/dosbox.Tpo .deps/dosbox.Po
g++  -g -O2   -o dosbox dosbox.o  cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a -lSDL_sound  -L/usr/lib -lSDL -lpng -lz -lSDL_net -lX11
ls
