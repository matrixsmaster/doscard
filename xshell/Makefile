## DosCard Project XShell MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

##TODO: make callcatcher work on AR

APP := xshell
CXX	:= g++-4.8.2
INCLUDES := -I. -I../box/include
DEFINES := -DXSHELL_VERBOSE=1
CXXFLAGS := -g -O2 -Wall $(INCLUDES) $(DEFINES)
LD_STAT := -ldosbox
LD_BDYN := -lSDL2 -lX11 -lrt -ldl -lpthread
LD_LIBS := -L../box/src -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
#-L../SDL2/build/.libs 
SUPPORTS := xsupport.o
##GLOB_OBJS := $(addprefix ../box/src/,$(shell cat ../box/src/objs.txt)) $(SUPPORTS)

all:	$(SUPPORTS) $(APP).o
	$(CXX) -g -o $(APP) $(SUPPORTS) $(APP).o $(LD_LIBS)
.PHONY:	all

#findunused:	$(SUPPORTS) $(APP).o
#	callarchive ../box/src/libdosbox.a $(APP).o
#	$(CXX) -g -o $(APP) $(SUPPORTS) $(APP).o $(LD_LIBS)
#	callanalyse $(APP)
#findunused:
#	echo $(GLOB_OBJS)
#	$(CXX) -g -o $(APP) $(APP).o $(GLOB_OBJS) $(LD_LIBS)
#	callanalyse $(APP)
#.PHONY: findunused

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP) *.o