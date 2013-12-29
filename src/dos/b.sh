#!/bin/sh
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
