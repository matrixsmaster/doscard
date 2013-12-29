#!/bin/sh
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT fpu.o -MD -MP -MF .deps/fpu.Tpo -c -o fpu.o fpu.cpp
mv -f .deps/fpu.Tpo .deps/fpu.Po
rm -f libfpu.a
ar cru libfpu.a fpu.o 
ranlib libfpu.a
