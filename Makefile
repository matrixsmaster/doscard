## DosCard Project XShell MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

##TODO: make callcatcher work on AR

APP := xshell
CXX := g++
#CALLCATCH := callcatcher
INCLUDES := -I. -I../../trunk/ -Iinclude
#DEFINES := -DXSHELL_VERBOSE=1
CXXFLAGS := -g -O0 -Wall $(INCLUDES) $(DEFINES)
LD_STAT := -ldosbox -lSDL2
LD_BDYN := -lX11 -lpthread -lrt
LD_LIBS := -L../../trunk/SDL2/build/.libs -Lsrc -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := fake_threading.o xsupport.o

all:	$(SUPPORTS) $(APP).o
	cd src; make all
	$(CXX) -g -o $(APP) $(SUPPORTS) $(APP).o $(LD_LIBS)
.PHONY:	all

findunusedcode:	$(SUPPORTS) $(APP).o
	callarchive ../box/src/libdosbox.a $(APP).o
	$(CALLCATCH) $(CXX) -g -o $(APP) $(SUPPORTS) $(APP).o $(LD_LIBS)
	callanalyse $(APP)
.PHONY: findunusedcode

%.o:	%.cpp
	$(CALLCATCH) $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	cd src; make clean
	rm -f $(APP) *.o
