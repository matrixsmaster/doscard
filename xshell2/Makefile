## DosCard Project XShell-2 MAKEFILE
## Copyright (C) 2013-2015 Soloviov Dmitry

APP	:= xshell2
CXX	:= g++-4.8.3
INCLUDES := -I. -I../box/include -I../libdoscard/include
DEFINES := 
CXXFLAGS := -g -O2 -Wall -Wextra $(INCLUDES) $(DEFINES)
LD_STAT := 
LD_BDYN := -ldoscard -lSDL2 -lX11 -lrt -lm -ldl -lpthread
LD_LIBS := -L../libdoscard/src -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := xsupport.o

all:	$(SUPPORTS) $(APP).o
	$(CXX) $(CXXFLAGS) -o $(APP) $(APP).o $(SUPPORTS) $(LD_LIBS)
.PHONY: all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APP)
