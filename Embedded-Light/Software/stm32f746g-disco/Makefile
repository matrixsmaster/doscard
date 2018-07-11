# STM32 ARM test Makefile
CRSS_P	:= /mnt/data/toolchain/gcc-arm-none-eabi-5_4-2016q3/bin
STLNK_P := /mnt/data/tools/stlink/build/Release
PREFIX  := $(CRSS_P)/arm-none-eabi
CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-g++
AS		:= $(PREFIX)-as
OBJCP	:= $(PREFIX)-objcopy
MNDIR	:= $(shell pwd)

APP		:= f7test.elf

DEPSRC	:= Src
DEPDRV	:= Drivers
DEPMDW	:= Middlewares

INCLS	:= -IInc
INCLS	+= $(addprefix -I,$(shell find $(DEPDRV) -type f -iname "*.h" | xargs dirname | sort -u))
INCLS	+= $(addprefix -I,$(shell find $(DEPMDW) -type f -iname "*.h" | xargs dirname | sort -u))

CHIP	:= -mcpu=cortex-m7 -mthumb
CHIP	+= -specs=nosys.specs
CHIP	+= -DSTM32F746xx
LINKSCR := -TSTM32F746NGHx_FLASH.ld
INIT	:= $(MNDIR)/startup_stm32f746xx.o

DBG		:= 
OPT		:= -O2
 
CC_DEFS := -D__thumb2__=1 -DRUN_FROM_FLASH=1

CC_FLAG := -std=gnu99 $(CHIP) $(DBG) $(OPT) -Wall -Wextra -ffunction-sections -fdata-sections -fno-strict-aliasing
CC_FLAG += -mno-sched-prolog -fno-hosted -fmessage-length=0
CC_FLAG += -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable

CXX_FLAG := -std=c++11 $(CHIP) $(DBG) $(OPT) -Wall -Wextra -ffunction-sections -fdata-sections -fno-strict-aliasing -fsigned-char
CXX_FLAG += -DUSE_MINI_THREADS

LD_FLAG := $(CHIP) $(DBG) $(OPT) -L$(DEPSRC) -Wl,--gc-sections
LD_FLAG += -mcpu=cortex-m7 -static -fwhole-program -Xlinker $(LINKSCR)
LD_FLAG += -lgcc -lc -lm -lrdimon -lstdc++ -lnosys
LD_FLAG += -Wl,--Map=$(APP).map

DEPS	:= $(shell find $(DEPSRC) -type f -iname '*.c')
DEPS	+= $(shell find $(DEPDRV) -type f -iname '*.c')
DEPS	+= $(shell find $(DEPMDW) -type f -iname '*.c')
DEPS_OBJ:= $(DEPS:.c=.o) 

CXX_DEPS := $(shell find $(DEPSRC) -type f -iname '*.cpp')
DEPS_OBJ+= $(CXX_DEPS:.cpp=.o)

CC_INV	:= $(CC) $(CC_FLAG) $(CC_DEFS) $(INCLS)
CXX_INV	:= $(CXX) $(CXX_FLAG) $(CC_DEFS) $(INCLS)

all:	$(DEPS_OBJ) $(INIT)
	$(LD) $(LD_FLAG) -o $(APP) $(DEPS_OBJ) $(INIT)
	$(OBJCP) -O binary $(APP) $(APP:.elf=.bin)
	$(OBJCP) -O ihex $(APP) $(APP:.elf=.hex)
.PHONY:	all

clean:
	rm -f $(DEPS_OBJ)
	rm -f $(PROG_OBJ)
	rm -f $(INIT)
	rm -f $(APP) $(APP:.elf=.bin) $(APP:.elf=.hex) $(APP).map
.PHONY: clean

purge:
	find . -type f -iname '*.o' -delete
	sudo $(STLNK_P)/st-flash erase
.PHONY: purge

upload: all
	sudo $(STLNK_P)/st-flash --reset write $(APP:.elf=.bin) 0x08000000
.PHONY:	upload

gr_upload: all
	SUDO_ASKPASS=$(shell which ssh-askpass) sudo -A $(STLNK_P)/st-flash --reset write $(APP:.elf=.bin) 0x08000000
.PHONY:	gr_upload

%.o:	%.c
	$(CC_INV) -c -o $@ $<
	
%.o:	%.cpp
	$(CXX_INV) -c -o $@ $<

%.o:	%.s
	$(AS) -o $@ $<
