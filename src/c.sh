#!/bin/sh

find . -type d -name 'build' | xargs -n1 rm -rfv
find . -type f -name '*.a' | xargs -n1 rm -rfv
find . -type f -name '*.o' | xargs -n1 rm -rfv
rm -f dosbox

cd cpu
mkdir build
echo CPU
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT callback.o -MD -MP -MF build/callback.Tpo -c -o callback.o callback.cpp
mv -f build/callback.Tpo build/callback.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cpu.o -MD -MP -MF build/cpu.Tpo -c -o cpu.o cpu.cpp
mv -f build/cpu.Tpo build/cpu.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT flags.o -MD -MP -MF build/flags.Tpo -c -o flags.o flags.cpp
mv -f build/flags.Tpo build/flags.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT modrm.o -MD -MP -MF build/modrm.Tpo -c -o modrm.o modrm.cpp
mv -f build/modrm.Tpo build/modrm.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_full.o -MD -MP -MF build/core_full.Tpo -c -o core_full.o core_full.cpp
mv -f build/core_full.Tpo build/core_full.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT paging.o -MD -MP -MF build/paging.Tpo -c -o paging.o paging.cpp
mv -f build/paging.Tpo build/paging.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_normal.o -MD -MP -MF build/core_normal.Tpo -c -o core_normal.o core_normal.cpp
mv -f build/core_normal.Tpo build/core_normal.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_simple.o -MD -MP -MF build/core_simple.Tpo -c -o core_simple.o core_simple.cpp
mv -f build/core_simple.Tpo build/core_simple.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_prefetch.o -MD -MP -MF build/core_prefetch.Tpo -c -o core_prefetch.o core_prefetch.cpp
mv -f build/core_prefetch.Tpo build/core_prefetch.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_dyn_x86.o -MD -MP -MF build/core_dyn_x86.Tpo -c -o core_dyn_x86.o core_dyn_x86.cpp
mv -f build/core_dyn_x86.Tpo build/core_dyn_x86.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT core_dynrec.o -MD -MP -MF build/core_dynrec.Tpo -c -o core_dynrec.o core_dynrec.cpp
mv -f build/core_dynrec.Tpo build/core_dynrec.Po
rm -f libcpu.a
ar cru libcpu.a callback.o cpu.o flags.o modrm.o core_full.o paging.o core_normal.o core_simple.o core_prefetch.o core_dyn_x86.o core_dynrec.o 
ranlib libcpu.a

cd ../fpu
mkdir build
echo FPU
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT fpu.o -MD -MP -MF build/fpu.Tpo -c -o fpu.o fpu.cpp
mv -f build/fpu.Tpo build/fpu.Po
rm -f libfpu.a
ar cru libfpu.a fpu.o 
ranlib libfpu.a

cd ../debug
mkdir build
echo DBG
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug.o -MD -MP -MF build/debug.Tpo -c -o debug.o debug.cpp
mv -f build/debug.Tpo build/debug.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_gui.o -MD -MP -MF build/debug_gui.Tpo -c -o debug_gui.o debug_gui.cpp
mv -f build/debug_gui.Tpo build/debug_gui.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_disasm.o -MD -MP -MF build/debug_disasm.Tpo -c -o debug_disasm.o debug_disasm.cpp
mv -f build/debug_disasm.Tpo build/debug_disasm.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT debug_win32.o -MD -MP -MF build/debug_win32.Tpo -c -o debug_win32.o debug_win32.cpp
mv -f build/debug_win32.Tpo build/debug_win32.Po
rm -f libdebug.a
ar cru libdebug.a debug.o debug_gui.o debug_disasm.o debug_win32.o 
ranlib libdebug.a

cd ../dos
mkdir build
echo DOS
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos.o -MD -MP -MF build/dos.Tpo -c -o dos.o dos.cpp
mv -f build/dos.Tpo build/dos.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_devices.o -MD -MP -MF build/dos_devices.Tpo -c -o dos_devices.o dos_devices.cpp
mv -f build/dos_devices.Tpo build/dos_devices.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_execute.o -MD -MP -MF build/dos_execute.Tpo -c -o dos_execute.o dos_execute.cpp
mv -f build/dos_execute.Tpo build/dos_execute.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_files.o -MD -MP -MF build/dos_files.Tpo -c -o dos_files.o dos_files.cpp
mv -f build/dos_files.Tpo build/dos_files.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_ioctl.o -MD -MP -MF build/dos_ioctl.Tpo -c -o dos_ioctl.o dos_ioctl.cpp
mv -f build/dos_ioctl.Tpo build/dos_ioctl.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_memory.o -MD -MP -MF build/dos_memory.Tpo -c -o dos_memory.o dos_memory.cpp
mv -f build/dos_memory.Tpo build/dos_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_misc.o -MD -MP -MF build/dos_misc.Tpo -c -o dos_misc.o dos_misc.cpp
mv -f build/dos_misc.Tpo build/dos_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_classes.o -MD -MP -MF build/dos_classes.Tpo -c -o dos_classes.o dos_classes.cpp
mv -f build/dos_classes.Tpo build/dos_classes.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_programs.o -MD -MP -MF build/dos_programs.Tpo -c -o dos_programs.o dos_programs.cpp
mv -f build/dos_programs.Tpo build/dos_programs.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_tables.o -MD -MP -MF build/dos_tables.Tpo -c -o dos_tables.o dos_tables.cpp
mv -f build/dos_tables.Tpo build/dos_tables.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drives.o -MD -MP -MF build/drives.Tpo -c -o drives.o drives.cpp
mv -f build/drives.Tpo build/drives.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_virtual.o -MD -MP -MF build/drive_virtual.Tpo -c -o drive_virtual.o drive_virtual.cpp
mv -f build/drive_virtual.Tpo build/drive_virtual.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_local.o -MD -MP -MF build/drive_local.Tpo -c -o drive_local.o drive_local.cpp
mv -f build/drive_local.Tpo build/drive_local.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_cache.o -MD -MP -MF build/drive_cache.Tpo -c -o drive_cache.o drive_cache.cpp
mv -f build/drive_cache.Tpo build/drive_cache.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_fat.o -MD -MP -MF build/drive_fat.Tpo -c -o drive_fat.o drive_fat.cpp
mv -f build/drive_fat.Tpo build/drive_fat.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT drive_iso.o -MD -MP -MF build/drive_iso.Tpo -c -o drive_iso.o drive_iso.cpp
mv -f build/drive_iso.Tpo build/drive_iso.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_mscdex.o -MD -MP -MF build/dos_mscdex.Tpo -c -o dos_mscdex.o dos_mscdex.cpp
mv -f build/dos_mscdex.Tpo build/dos_mscdex.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dos_keyboard_layout.o -MD -MP -MF build/dos_keyboard_layout.Tpo -c -o dos_keyboard_layout.o dos_keyboard_layout.cpp
mv -f build/dos_keyboard_layout.Tpo build/dos_keyboard_layout.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom.o -MD -MP -MF build/cdrom.Tpo -c -o cdrom.o cdrom.cpp
mv -f build/cdrom.Tpo build/cdrom.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_win32.o -MD -MP -MF build/cdrom_ioctl_win32.Tpo -c -o cdrom_ioctl_win32.o cdrom_ioctl_win32.cpp
mv -f build/cdrom_ioctl_win32.Tpo build/cdrom_ioctl_win32.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_aspi_win32.o -MD -MP -MF build/cdrom_aspi_win32.Tpo -c -o cdrom_aspi_win32.o cdrom_aspi_win32.cpp
mv -f build/cdrom_aspi_win32.Tpo build/cdrom_aspi_win32.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_linux.o -MD -MP -MF build/cdrom_ioctl_linux.Tpo -c -o cdrom_ioctl_linux.o cdrom_ioctl_linux.cpp
mv -f build/cdrom_ioctl_linux.Tpo build/cdrom_ioctl_linux.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_image.o -MD -MP -MF build/cdrom_image.Tpo -c -o cdrom_image.o cdrom_image.cpp
mv -f build/cdrom_image.Tpo build/cdrom_image.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cdrom_ioctl_os2.o -MD -MP -MF build/cdrom_ioctl_os2.Tpo -c -o cdrom_ioctl_os2.o cdrom_ioctl_os2.cpp
mv -f build/cdrom_ioctl_os2.Tpo build/cdrom_ioctl_os2.Po
rm -f libdos.a
ar cru libdos.a dos.o dos_devices.o dos_execute.o dos_files.o dos_ioctl.o dos_memory.o dos_misc.o dos_classes.o dos_programs.o dos_tables.o drives.o drive_virtual.o drive_local.o drive_cache.o drive_fat.o drive_iso.o dos_mscdex.o dos_keyboard_layout.o cdrom.o cdrom_ioctl_win32.o cdrom_aspi_win32.o cdrom_ioctl_linux.o cdrom_image.o cdrom_ioctl_os2.o 
ranlib libdos.a

cd ../hardware
mkdir build
echo HDW
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT adlib.o -MD -MP -MF build/adlib.Tpo -c -o adlib.o adlib.cpp
mv -f build/adlib.Tpo build/adlib.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dma.o -MD -MP -MF build/dma.Tpo -c -o dma.o dma.cpp
mv -f build/dma.Tpo build/dma.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gameblaster.o -MD -MP -MF build/gameblaster.Tpo -c -o gameblaster.o gameblaster.cpp
mv -f build/gameblaster.Tpo build/gameblaster.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT hardware.o -MD -MP -MF build/hardware.Tpo -c -o hardware.o hardware.cpp
mv -f build/hardware.Tpo build/hardware.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT iohandler.o -MD -MP -MF build/iohandler.Tpo -c -o iohandler.o iohandler.cpp
mv -f build/iohandler.Tpo build/iohandler.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT joystick.o -MD -MP -MF build/joystick.Tpo -c -o joystick.o joystick.cpp
mv -f build/joystick.Tpo build/joystick.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT keyboard.o -MD -MP -MF build/keyboard.Tpo -c -o keyboard.o keyboard.cpp
mv -f build/keyboard.Tpo build/keyboard.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT memory.o -MD -MP -MF build/memory.Tpo -c -o memory.o memory.cpp
mv -f build/memory.Tpo build/memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mixer.o -MD -MP -MF build/mixer.Tpo -c -o mixer.o mixer.cpp
mv -f build/mixer.Tpo build/mixer.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pcspeaker.o -MD -MP -MF build/pcspeaker.Tpo -c -o pcspeaker.o pcspeaker.cpp
mv -f build/pcspeaker.Tpo build/pcspeaker.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pci_bus.o -MD -MP -MF build/pci_bus.Tpo -c -o pci_bus.o pci_bus.cpp
mv -f build/pci_bus.Tpo build/pci_bus.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT pic.o -MD -MP -MF build/pic.Tpo -c -o pic.o pic.cpp
mv -f build/pic.Tpo build/pic.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sblaster.o -MD -MP -MF build/sblaster.Tpo -c -o sblaster.o sblaster.cpp
mv -f build/sblaster.Tpo build/sblaster.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT tandy_sound.o -MD -MP -MF build/tandy_sound.Tpo -c -o tandy_sound.o tandy_sound.cpp
mv -f build/tandy_sound.Tpo build/tandy_sound.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT timer.o -MD -MP -MF build/timer.Tpo -c -o timer.o timer.cpp
mv -f build/timer.Tpo build/timer.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga.o -MD -MP -MF build/vga.Tpo -c -o vga.o vga.cpp
mv -f build/vga.Tpo build/vga.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_attr.o -MD -MP -MF build/vga_attr.Tpo -c -o vga_attr.o vga_attr.cpp
mv -f build/vga_attr.Tpo build/vga_attr.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_crtc.o -MD -MP -MF build/vga_crtc.Tpo -c -o vga_crtc.o vga_crtc.cpp
mv -f build/vga_crtc.Tpo build/vga_crtc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_dac.o -MD -MP -MF build/vga_dac.Tpo -c -o vga_dac.o vga_dac.cpp
mv -f build/vga_dac.Tpo build/vga_dac.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_draw.o -MD -MP -MF build/vga_draw.Tpo -c -o vga_draw.o vga_draw.cpp
mv -f build/vga_draw.Tpo build/vga_draw.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_gfx.o -MD -MP -MF build/vga_gfx.Tpo -c -o vga_gfx.o vga_gfx.cpp
mv -f build/vga_gfx.Tpo build/vga_gfx.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_other.o -MD -MP -MF build/vga_other.Tpo -c -o vga_other.o vga_other.cpp
mv -f build/vga_other.Tpo build/vga_other.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_memory.o -MD -MP -MF build/vga_memory.Tpo -c -o vga_memory.o vga_memory.cpp
mv -f build/vga_memory.Tpo build/vga_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_misc.o -MD -MP -MF build/vga_misc.Tpo -c -o vga_misc.o vga_misc.cpp
mv -f build/vga_misc.Tpo build/vga_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_seq.o -MD -MP -MF build/vga_seq.Tpo -c -o vga_seq.o vga_seq.cpp
mv -f build/vga_seq.Tpo build/vga_seq.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_xga.o -MD -MP -MF build/vga_xga.Tpo -c -o vga_xga.o vga_xga.cpp
mv -f build/vga_xga.Tpo build/vga_xga.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_s3.o -MD -MP -MF build/vga_s3.Tpo -c -o vga_s3.o vga_s3.cpp
mv -f build/vga_s3.Tpo build/vga_s3.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_tseng.o -MD -MP -MF build/vga_tseng.Tpo -c -o vga_tseng.o vga_tseng.cpp
mv -f build/vga_tseng.Tpo build/vga_tseng.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT vga_paradise.o -MD -MP -MF build/vga_paradise.Tpo -c -o vga_paradise.o vga_paradise.cpp
mv -f build/vga_paradise.Tpo build/vga_paradise.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cmos.o -MD -MP -MF build/cmos.Tpo -c -o cmos.o cmos.cpp
mv -f build/cmos.Tpo build/cmos.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT disney.o -MD -MP -MF build/disney.Tpo -c -o disney.o disney.cpp
mv -f build/disney.Tpo build/disney.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gus.o -MD -MP -MF build/gus.Tpo -c -o gus.o gus.cpp
mv -f build/gus.Tpo build/gus.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mpu401.o -MD -MP -MF build/mpu401.Tpo -c -o mpu401.o mpu401.cpp
mv -f build/mpu401.Tpo build/mpu401.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ipx.o -MD -MP -MF build/ipx.Tpo -c -o ipx.o ipx.cpp
mv -f build/ipx.Tpo build/ipx.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ipxserver.o -MD -MP -MF build/ipxserver.Tpo -c -o ipxserver.o ipxserver.cpp
mv -f build/ipxserver.Tpo build/ipxserver.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dbopl.o -MD -MP -MF build/dbopl.Tpo -c -o dbopl.o dbopl.cpp
mv -f build/dbopl.Tpo build/dbopl.Po
rm -f libhardware.a
ar cru libhardware.a adlib.o dma.o gameblaster.o hardware.o iohandler.o joystick.o keyboard.o memory.o mixer.o pcspeaker.o pci_bus.o pic.o sblaster.o tandy_sound.o timer.o vga.o vga_attr.o vga_crtc.o vga_dac.o vga_draw.o vga_gfx.o vga_other.o vga_memory.o vga_misc.o vga_seq.o vga_xga.o vga_s3.o vga_tseng.o vga_paradise.o cmos.o disney.o gus.o mpu401.o ipx.o ipxserver.o dbopl.o 
ranlib libhardware.a

cd serialport
mkdir build
echo TTY
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT directserial.o -MD -MP -MF build/directserial.Tpo -c -o directserial.o directserial.cpp
mv -f build/directserial.Tpo build/directserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT libserial.o -MD -MP -MF build/libserial.Tpo -c -o libserial.o libserial.cpp
mv -f build/libserial.Tpo build/libserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialdummy.o -MD -MP -MF build/serialdummy.Tpo -c -o serialdummy.o serialdummy.cpp
mv -f build/serialdummy.Tpo build/serialdummy.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialport.o -MD -MP -MF build/serialport.Tpo -c -o serialport.o serialport.cpp
mv -f build/serialport.Tpo build/serialport.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT softmodem.o -MD -MP -MF build/softmodem.Tpo -c -o softmodem.o softmodem.cpp
mv -f build/softmodem.Tpo build/softmodem.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT misc_util.o -MD -MP -MF build/misc_util.Tpo -c -o misc_util.o misc_util.cpp
mv -f build/misc_util.Tpo build/misc_util.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT nullmodem.o -MD -MP -MF build/nullmodem.Tpo -c -o nullmodem.o nullmodem.cpp
mv -f build/nullmodem.Tpo build/nullmodem.Po
rm -f libserial.a
ar cru libserial.a directserial.o libserial.o serialdummy.o serialport.o softmodem.o misc_util.o nullmodem.o 
ranlib libserial.a

cd ../../ints
mkdir build
echo IRQ
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT mouse.o -MD -MP -MF build/mouse.Tpo -c -o mouse.o mouse.cpp
mv -f build/mouse.Tpo build/mouse.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT xms.o -MD -MP -MF build/xms.Tpo -c -o xms.o xms.cpp
mv -f build/xms.Tpo build/xms.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT ems.o -MD -MP -MF build/ems.Tpo -c -o ems.o ems.cpp
mv -f build/ems.Tpo build/ems.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10.o -MD -MP -MF build/int10.Tpo -c -o int10.o int10.cpp
mv -f build/int10.Tpo build/int10.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_char.o -MD -MP -MF build/int10_char.Tpo -c -o int10_char.o int10_char.cpp
mv -f build/int10_char.Tpo build/int10_char.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_memory.o -MD -MP -MF build/int10_memory.Tpo -c -o int10_memory.o int10_memory.cpp
mv -f build/int10_memory.Tpo build/int10_memory.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_misc.o -MD -MP -MF build/int10_misc.Tpo -c -o int10_misc.o int10_misc.cpp
mv -f build/int10_misc.Tpo build/int10_misc.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_modes.o -MD -MP -MF build/int10_modes.Tpo -c -o int10_modes.o int10_modes.cpp
mv -f build/int10_modes.Tpo build/int10_modes.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_vesa.o -MD -MP -MF build/int10_vesa.Tpo -c -o int10_vesa.o int10_vesa.cpp
mv -f build/int10_vesa.Tpo build/int10_vesa.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_pal.o -MD -MP -MF build/int10_pal.Tpo -c -o int10_pal.o int10_pal.cpp
mv -f build/int10_pal.Tpo build/int10_pal.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_put_pixel.o -MD -MP -MF build/int10_put_pixel.Tpo -c -o int10_put_pixel.o int10_put_pixel.cpp
mv -f build/int10_put_pixel.Tpo build/int10_put_pixel.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_video_state.o -MD -MP -MF build/int10_video_state.Tpo -c -o int10_video_state.o int10_video_state.cpp
mv -f build/int10_video_state.Tpo build/int10_video_state.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT int10_vptable.o -MD -MP -MF build/int10_vptable.Tpo -c -o int10_vptable.o int10_vptable.cpp
mv -f build/int10_vptable.Tpo build/int10_vptable.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios.o -MD -MP -MF build/bios.Tpo -c -o bios.o bios.cpp
mv -f build/bios.Tpo build/bios.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios_disk.o -MD -MP -MF build/bios_disk.Tpo -c -o bios_disk.o bios_disk.cpp
mv -f build/bios_disk.Tpo build/bios_disk.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT bios_keyboard.o -MD -MP -MF build/bios_keyboard.Tpo -c -o bios_keyboard.o bios_keyboard.cpp
mv -f build/bios_keyboard.Tpo build/bios_keyboard.Po
rm -f libints.a
ar cru libints.a mouse.o xms.o ems.o int10.o int10_char.o int10_memory.o int10_misc.o int10_modes.o int10_vesa.o int10_pal.o int10_put_pixel.o int10_video_state.o int10_vptable.o bios.o bios_disk.o bios_keyboard.o 
ranlib libints.a

cd ../libs/gui_tk
mkdir build
echo GTK
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gui_tk.o -MD -MP -MF build/gui_tk.Tpo -c -o gui_tk.o gui_tk.cpp
mv -f build/gui_tk.Tpo build/gui_tk.Po
rm -f libgui_tk.a
ar cru libgui_tk.a gui_tk.o 
ranlib libgui_tk.a

cd ../../misc
mkdir build
echo MSC
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT cross.o -MD -MP -MF build/cross.Tpo -c -o cross.o cross.cpp
mv -f build/cross.Tpo build/cross.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT messages.o -MD -MP -MF build/messages.Tpo -c -o messages.o messages.cpp
mv -f build/messages.Tpo build/messages.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT programs.o -MD -MP -MF build/programs.Tpo -c -o programs.o programs.cpp
mv -f build/programs.Tpo build/programs.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT setup.o -MD -MP -MF build/setup.Tpo -c -o setup.o setup.cpp
mv -f build/setup.Tpo build/setup.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT support.o -MD -MP -MF build/support.Tpo -c -o support.o support.cpp
mv -f build/support.Tpo build/support.Po
rm -f libmisc.a
ar cru libmisc.a cross.o messages.o programs.o setup.o support.o 
ranlib libmisc.a

cd ../shell
mkdir build
echo SHL
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell.o -MD -MP -MF build/shell.Tpo -c -o shell.o shell.cpp
mv -f build/shell.Tpo build/shell.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_batch.o -MD -MP -MF build/shell_batch.Tpo -c -o shell_batch.o shell_batch.cpp
mv -f build/shell_batch.Tpo build/shell_batch.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_cmds.o -MD -MP -MF build/shell_cmds.Tpo -c -o shell_cmds.o shell_cmds.cpp
mv -f build/shell_cmds.Tpo build/shell_cmds.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT shell_misc.o -MD -MP -MF build/shell_misc.Tpo -c -o shell_misc.o shell_misc.cpp
mv -f build/shell_misc.Tpo build/shell_misc.Po
rm -f libshell.a
ar cru libshell.a shell.o shell_batch.o shell_cmds.o shell_misc.o 
ranlib libshell.a

cd ../gui
mkdir build
echo GUI
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdlmain.o -MD -MP -MF build/sdlmain.Tpo -c -o sdlmain.o sdlmain.cpp
mv -f build/sdlmain.Tpo build/sdlmain.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_mapper.o -MD -MP -MF build/sdl_mapper.Tpo -c -o sdl_mapper.o sdl_mapper.cpp
mv -f build/sdl_mapper.Tpo build/sdl_mapper.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render.o -MD -MP -MF build/render.Tpo -c -o render.o render.cpp
mv -f build/render.Tpo build/render.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render_scalers.o -MD -MP -MF build/render_scalers.Tpo -c -o render_scalers.o render_scalers.cpp
mv -f build/render_scalers.Tpo build/render_scalers.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT midi.o -MD -MP -MF build/midi.Tpo -c -o midi.o midi.cpp
mv -f build/midi.Tpo build/midi.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_gui.o -MD -MP -MF build/sdl_gui.Tpo -c -o sdl_gui.o sdl_gui.cpp
mv -f build/sdl_gui.Tpo build/sdl_gui.Po
rm -f libgui.a
ar cru libgui.a sdlmain.o sdl_mapper.o render.o render_scalers.o midi.o sdl_gui.o 
ranlib libgui.a

cd ..
mkdir build
echo MAIN
g++ -DHAVE_CONFIG_H -I. -I..  -I../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT dosbox.o -MD -MP -MF build/dosbox.Tpo -c -o dosbox.o dosbox.cpp
mv -f build/dosbox.Tpo build/dosbox.Po
g++  -g -O2   -o dosbox dosbox.o  cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a -lSDL_sound  -L/usr/lib -lSDL -lpng -lz -lSDL_net -lX11
ls
