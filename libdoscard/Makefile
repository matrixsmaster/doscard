## DosCard Project LibDOSCard MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

LIB := libdoscard
CXX	:= g++-4.8.3
WRAPSRC  := wrapper_pc
WRAPLIB  := libdbwrap.bc
INCLUDES := -Iinclude -I$(WRAPSRC)
DEFINES := -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
BLDINFO := -DBUILDATE="\"$(shell date)\""
BLDINFO += -DCOMPILERNAME="\"$(shell $(CXX) --version | head -n1)\""
BLDINFO += -DBUILDNUMBER="\"$(shell cat buildno)\""
DEFINES += $(BLDINFO)
CXXFLAGS := -g -O2 -fPIC -Wall -Wextra $(INCLUDES) $(DEFINES)
LD_STAT := 
LD_BDYN := -lLLVM-3.3 -lpthread -ldl -lrt
LD_LIBS := -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := doscard.o

all:	$(WRAPLIB) $(SUPPORTS)
	$(CXX) -shared $(CXXFLAGS) -o $(LIB).so $(SUPPORTS) $(LD_LIBS)
	expr `cat buildno` + 1 > buildno
.PHONY: all

$(WRAPLIB):
	cd $(WRAPSRC); make all
	cp -a $(WRAPSRC)/$(WRAPLIB) ./

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	cd $(WRAPSRC); make clean
	rm -f *.o *.bc *.so
