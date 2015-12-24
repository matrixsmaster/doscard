# 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
# Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
#
# This work is licensed under the MIT License. See included LICENSE.TXT.

CXXFLAGS=-O2 -Wall -Wsign-compare -DNO_GRAPHICS

APP=t86
OBJS=VM86.o VM86EU.o VM86L.o test.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(APP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJS)
