#!/bin/sh
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdlmain.o -MD -MP -MF .deps/sdlmain.Tpo -c -o sdlmain.o sdlmain.cpp
mv -f .deps/sdlmain.Tpo .deps/sdlmain.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_mapper.o -MD -MP -MF .deps/sdl_mapper.Tpo -c -o sdl_mapper.o sdl_mapper.cpp
mv -f .deps/sdl_mapper.Tpo .deps/sdl_mapper.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render.o -MD -MP -MF .deps/render.Tpo -c -o render.o render.cpp
mv -f .deps/render.Tpo .deps/render.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT render_scalers.o -MD -MP -MF .deps/render_scalers.Tpo -c -o render_scalers.o render_scalers.cpp
mv -f .deps/render_scalers.Tpo .deps/render_scalers.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT midi.o -MD -MP -MF .deps/midi.Tpo -c -o midi.o midi.cpp
mv -f .deps/midi.Tpo .deps/midi.Po
g++ -DHAVE_CONFIG_H -I. -I../..  -I../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT sdl_gui.o -MD -MP -MF .deps/sdl_gui.Tpo -c -o sdl_gui.o sdl_gui.cpp
mv -f .deps/sdl_gui.Tpo .deps/sdl_gui.Po
rm -f libgui.a
ar cru libgui.a sdlmain.o sdl_mapper.o render.o render_scalers.o midi.o sdl_gui.o 
ranlib libgui.a
