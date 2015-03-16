## DosCard Project LibDOSCard WinWrap Makefile
## Copyright (C) 2015 Dmitry Soloviov

LIBNAME	 := libwinwrap
CXX		 := g++

INCLUDES := -I../../include -I ../../../box/include

DEFINES  :=

CXXFLAGS := -c -g -O2 $(INCLUDES) $(DEFINES)
LD_LIBS  := 
SUPPORTS := winwrap.o


all:	$(SUPPORTS)
	rm -f $(LIBNAME).a
	ar cru $(LIBNAME).a $(SUPPORTS)

.PHONY: all

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.a
