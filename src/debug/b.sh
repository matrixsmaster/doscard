#!/bin/sh
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
