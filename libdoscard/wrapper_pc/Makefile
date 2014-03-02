## DosCard Project DosCard PC Wrapper for LLVM
## Copyright (C) 2013-2014 Soloviov Dmitry

LIBNAME	 := libdbwrap
CXX		 := clang++
INCLUDES := -I. -I.. -I../../box/include
DEFINES  := 
BLDINFO := -DBUILDATE="\"$(shell date)\""
BLDINFO += -DCOMPILERNAME="\"$(shell $(CXX) --version | head -n1)\""
DEFINES += $(BLDINFO)
CXXFLAGS := -c -emit-llvm -g -O2 -Wall $(INCLUDES) $(DEFINES)
#LD_STAT  := -ldosbox-llvm
#LD_BDYN  := 
LD_LIBS  := ../../box/src/libdosbox-llvm.bc
## -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
#LD_LIBS  := $(addprefix ../../box/src/,$(shell cat ../../box/src/objs.txt))
SUPPORTS := pchost.o ldbwrap.o

all:	$(SUPPORTS)
	cd ../../box; make llvm
	llvm-link -o $(LIBNAME).bc $(SUPPORTS) $(LD_LIBS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	rm -f *.bc *.o
