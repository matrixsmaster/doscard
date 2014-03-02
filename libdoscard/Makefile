## DosCard Project LibDOSCard MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

LIB := libdoscard
CXX	:= g++-4.8.2
INCLUDES := -I. -I.. -I../box/include
DEFINES := 
BLDINFO := -DBUILDATE="\"$(shell date)\""
BLDINFO += -DCOMPILERNAME="\"$(shell $(CXX) --version | head -n1)\""
BLDINFO += -DBUILDNUMBER="\"$(shell cat buildno)\""
DEFINES += $(BLDINFO)
CXXFLAGS := -g -O2 -Wall -Wextra $(INCLUDES) $(DEFINES)
#LD_STAT := 
#LD_BDYN := 
#LD_LIBS := -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
WRAP_SRC := pchost.cpp ldbwrap.cpp
CURWRAP  := wrapper_pc
SUPPORTS := $(addprefix $(CURWRAP)/,$(WRAP_SRC:.cpp=.o))

all:	$(SUPPORTS)
	$(CXX) -shared $(CXXFLAGS) -o $(LIB).so $(SUPPORTS) $(LD_LIBS)
.PHONY: all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	cd ../box; make clean
