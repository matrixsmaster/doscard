#!/bin/sh
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

