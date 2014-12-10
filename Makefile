# STM32 ARM test Makefile
# (C) Dmitry 'MatrixS_Master' Soloviov, 2014

CRSS_P	:= /mnt/angar/gcc-arm-none-eabi-4_8-2014q1/bin
PREFIX  := $(CRSS_P)/arm-none-eabi
CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-g++
AS		:= $(PREFIX)-as
OBJCP	:= $(PREFIX)-objcopy
MNDIR	:= $(shell pwd)

APP		:= rtos.elf
PROG	:= main.cpp

INCLS	:= -IInc
INCLS	+= $(addprefix -I,$(shell find Drivers -type f -iname "*.h" | xargs dirname | sort -u))
INCLS	+= $(addprefix -I,$(shell find Middlewares -type f -iname "*.h" | xargs dirname | sort -u))
DEPSRC	:= Src
DEPDRV	:= Drivers
DEPMDW	:= Middlewares

CHIP	:= -DSTM32F429xx
CHIP	+= -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LINKSCR := -Tf429flash.ld
INIT	:= $(MNDIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f429xx.o

DBG		:= -g
OPT		:= -Os
 
CC_DEFS := -DUSE_STDPERIPH_DRIVER -D__thumb2__=1 -DRUN_FROM_FLASH=1

CC_FLAG := $(CHIP) $(DBG) $(OPT) -Wall -Wextra -ffunction-sections -fdata-sections -fomit-frame-pointer 
CC_FLAG += -std=gnu99 -mno-sched-prolog -fno-hosted -fmessage-length=0
CC_FLAG += -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable

LD_FLAG := $(CHIP) $(DBG) $(OPT) -L$(DEPSRC) -Wl,--gc-sections
LD_FLAG += -mcpu=cortex-m4 -static -fwhole-program -Xlinker $(LINKSCR)

BUBEN	:= --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -lstdc++ -lnosys -Wl,--end-group

DEPS	:= $(shell find $(DEPSRC) -type f -iname '*.c')
DEPS	+= $(shell find $(DEPDRV) -type f -iname '*.c')
DEPS	+= $(shell find $(DEPMDW) -type f -iname '*.c')
DEPS_OBJ:= $(DEPS:.c=.o)

DOS		:= $(shell find dos -type f -iname '*.cpp')
DOS_INC := -I$(MNDIR)/dos/include
DOS_OBJ := $(DOS:.cpp=.o)
DOS_DEF := -DLDB_EMBEDDED=1
DOS_FLG := -Wall -fPIC -ffunction-sections -fdata-sections -fomit-frame-pointer

CC_INV	:= $(CC) $(CC_DEFS) $(INCLS) $(CC_FLAG)
CXX_INV := $(CXX) $(DBG) $(OPT) $(CHIP) $(DOS_DEF) $(DOS_FLG) $(DOS_INC)

PROG_OBJ:= $(PROG:.cpp=.o)

all:	$(INIT) main dos driver
	$(LD) $(LD_FLAG) $(BUBEN) -o $(APP) $(INIT) $(DEPS_OBJ) $(PROG_OBJ) $(DOS_OBJ)
	$(OBJCP) -O binary $(APP) $(APP:.elf=.bin)
.PHONY:	all

clean:
	rm -f $(DOS_OBJ)
	rm -f $(DEPS_OBJ)
	rm -f $(PROG_OBJ)
	rm -f $(INIT)
	rm -f $(APP) $(APP:.elf=.bin)
.PHONY: clean

main:
	$(CXX_INV) $(INCLS) -o $(PROG_OBJ) -c $(PROG)
.PHONY: main

dos: $(DOS_OBJ)

driver:	$(DEPS_OBJ)


%.o:	%.c
	$(CC_INV) -c -o $@ $<

%.o:	%.s
	$(AS) -o $@ $<

%.o:	%.cpp
	$(CXX_INV) -c -o $@ $<
