## Part of DosCard Project
## Copyright (C) 2013-2015 Dmitry Soloviov

APP	:= xtest
CXX	:= g++
INCLUDES := -I. -I../box/include -I../libdoscard/include
DEFINES := -DBITFILE_ALTPATH=\"../libdoscard/src/libdbwrap.bc\"
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
