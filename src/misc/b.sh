#!/bin/sh
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
