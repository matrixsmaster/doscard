## DosCard Project XS Headless Test MAKEFILE
## Copyright (C) 2013-2014 Dmitry Soloviov

APP	:= headless
CXX	:= g++-4.8.3
INCLUDES := -I. -I../box/include -I../libdoscard/include
DEFINES := 
CXXFLAGS := -g -O0 -Wall -Wextra $(INCLUDES) $(DEFINES)
LD_STAT := 
LD_BDYN := -ldoscard -lrt -lm -ldl -lpthread
LD_LIBS := -L../libdoscard/src -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := 

all:	$(SUPPORTS) $(APP).o
	$(CXX) $(CXXFLAGS) -o $(APP) $(APP).o $(SUPPORTS) $(LD_LIBS)
.PHONY: all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APP)
