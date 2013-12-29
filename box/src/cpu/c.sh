#!/bin/sh
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
rm -f libcpu.a
ar cru libcpu.a callback.o cpu.o flags.o modrm.o core_full.o paging.o core_normal.o core_simple.o core_prefetch.o 
ranlib libcpu.a

