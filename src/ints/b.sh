#!/bin/sh
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
