APP	:= dosbox
CC_DEFS := -DHAVE_CONFIG_H -D_GNU_SOURCE=1 -D_REENTRANT
CC_FLAG := -g -O2
CC_INCL := -I. -I../include
CC_COMP := g++ $(CC_DEFS) $(CC_FLAG) $(CC_INCL)
LD_LIBS := -lSDL_sound -lSDL -lpng -lz -lX11 -lcurses
BLDIR := build
MNDIR := $(shell pwd)

LCPU_SRC := callback.cpp cpu.cpp flags.cpp modrm.cpp core_full.cpp paging.cpp core_normal.cpp core_simple.cpp core_prefetch.cpp


LCPU_OBJ := $(LCPU_SRC:.cpp=.o)
LCPU_INS := $(addprefix cpu/,$(LCPU_OBJ))

all:	$(APP)
.PHONY:	all

$(APP): lcpu
	cd $(MNDIR)

lcpu:	$(LCPU_INS)
	rm -f cpu/libcpu.a
	ar cru cpu/libcpu.a $(LCPU_INS) 
	ranlib cpu/libcpu.a

%.o:	%.cpp
	$(CC_COMP) -c -o $@ $<

clean:
	find . -type f -name '*.a' | xargs -n1 rm -fv
	find . -type f -name '*.o' | xargs -n1 rm -fv
	rm -f dosbox
