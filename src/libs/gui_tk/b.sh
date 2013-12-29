#!/bin/sh
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT gui_tk.o -MD -MP -MF .deps/gui_tk.Tpo -c -o gui_tk.o gui_tk.cpp
mv -f .deps/gui_tk.Tpo .deps/gui_tk.Po
rm -f libgui_tk.a
ar cru libgui_tk.a gui_tk.o 
ranlib libgui_tk.a
