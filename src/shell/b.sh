#!/bin/sh
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
