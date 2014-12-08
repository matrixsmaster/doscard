## DosCard Project XShell-2 MAKEFILE
## Copyright (C) 2013-2014 Dmitry Soloviov

APP	:= xshell2
CXX	:= g++-4.8.3
#SDLLIBS := -lSDL2 -lSDL2_ttf
SDLLIBS := -lSDL2
INCLUDES := -I. -I../box/include -I../libdoscard/include
DEFINES := 
CXXFLAGS := -g -O2 -Wall -Wextra $(INCLUDES) $(DEFINES)
LD_STAT := 
LD_BDYN := $(SDLLIBS) -ldoscard -lX11 -lrt -lm -ldl -lpthread
LD_LIBS := -L../libdoscard -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := xsupport.o

all:	$(SUPPORTS) $(APP).o
	$(CXX) $(CXXFLAGS) -o $(APP) $(APP).o $(SUPPORTS) $(LD_LIBS)
.PHONY: all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APP)
