#!/bin/sh
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT directserial.o -MD -MP -MF .deps/directserial.Tpo -c -o directserial.o directserial.cpp
mv -f .deps/directserial.Tpo .deps/directserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT libserial.o -MD -MP -MF .deps/libserial.Tpo -c -o libserial.o libserial.cpp
mv -f .deps/libserial.Tpo .deps/libserial.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialdummy.o -MD -MP -MF .deps/serialdummy.Tpo -c -o serialdummy.o serialdummy.cpp
mv -f .deps/serialdummy.Tpo .deps/serialdummy.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT serialport.o -MD -MP -MF .deps/serialport.Tpo -c -o serialport.o serialport.cpp
mv -f .deps/serialport.Tpo .deps/serialport.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT softmodem.o -MD -MP -MF .deps/softmodem.Tpo -c -o softmodem.o softmodem.cpp
mv -f .deps/softmodem.Tpo .deps/softmodem.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT misc_util.o -MD -MP -MF .deps/misc_util.Tpo -c -o misc_util.o misc_util.cpp
mv -f .deps/misc_util.Tpo .deps/misc_util.Po
g++ -DHAVE_CONFIG_H -I. -I../../..  -I../../../include -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT  -g -O2 -MT nullmodem.o -MD -MP -MF .deps/nullmodem.Tpo -c -o nullmodem.o nullmodem.cpp
mv -f .deps/nullmodem.Tpo .deps/nullmodem.Po
rm -f libserial.a
ar cru libserial.a directserial.o libserial.o serialdummy.o serialport.o softmodem.o misc_util.o nullmodem.o 
ranlib libserial.a
