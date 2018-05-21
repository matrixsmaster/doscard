# 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
# Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
#
# This work is licensed under the MIT License. See included LICENSE.TXT.

ASM=nasm
#CXX=/mnt/angar/toolchain/zpu-gcc-g++-multilib-linux64-20150428/bin/zpu-elf-g++
CXXFLAGS=-O0 -Wall -Wsign-compare -DNO_GRAPHICS -g

#APP=t86z.elf
APP=t86

OBJS=VM86.o VM86EU.o VM86L.o test.o

all: bios $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(APP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
bios: bios.asm
	$(ASM) $<
	echo "static const " 1> VM86bios.h
	xxd -i bios 1>> VM86bios.h

clean:
	rm -f $(APP) $(OBJS) bios
