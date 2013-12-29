#!/bin/sh
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
