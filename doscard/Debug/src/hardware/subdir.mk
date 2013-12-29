################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hardware/adlib.cpp \
../src/hardware/cmos.cpp \
../src/hardware/dbopl.cpp \
../src/hardware/disney.cpp \
../src/hardware/dma.cpp \
../src/hardware/gameblaster.cpp \
../src/hardware/gus.cpp \
../src/hardware/hardware.cpp \
../src/hardware/iohandler.cpp \
../src/hardware/ipx.cpp \
../src/hardware/ipxserver.cpp \
../src/hardware/joystick.cpp \
../src/hardware/keyboard.cpp \
../src/hardware/memory.cpp \
../src/hardware/mixer.cpp \
../src/hardware/mpu401.cpp \
../src/hardware/opl.cpp \
../src/hardware/pci_bus.cpp \
../src/hardware/pcspeaker.cpp \
../src/hardware/pic.cpp \
../src/hardware/sblaster.cpp \
../src/hardware/tandy_sound.cpp \
../src/hardware/timer.cpp \
../src/hardware/vga.cpp \
../src/hardware/vga_attr.cpp \
../src/hardware/vga_crtc.cpp \
../src/hardware/vga_dac.cpp \
../src/hardware/vga_draw.cpp \
../src/hardware/vga_gfx.cpp \
../src/hardware/vga_memory.cpp \
../src/hardware/vga_misc.cpp \
../src/hardware/vga_other.cpp \
../src/hardware/vga_paradise.cpp \
../src/hardware/vga_s3.cpp \
../src/hardware/vga_seq.cpp \
../src/hardware/vga_tseng.cpp \
../src/hardware/vga_xga.cpp 

OBJS += \
./src/hardware/adlib.o \
./src/hardware/cmos.o \
./src/hardware/dbopl.o \
./src/hardware/disney.o \
./src/hardware/dma.o \
./src/hardware/gameblaster.o \
./src/hardware/gus.o \
./src/hardware/hardware.o \
./src/hardware/iohandler.o \
./src/hardware/ipx.o \
./src/hardware/ipxserver.o \
./src/hardware/joystick.o \
./src/hardware/keyboard.o \
./src/hardware/memory.o \
./src/hardware/mixer.o \
./src/hardware/mpu401.o \
./src/hardware/opl.o \
./src/hardware/pci_bus.o \
./src/hardware/pcspeaker.o \
./src/hardware/pic.o \
./src/hardware/sblaster.o \
./src/hardware/tandy_sound.o \
./src/hardware/timer.o \
./src/hardware/vga.o \
./src/hardware/vga_attr.o \
./src/hardware/vga_crtc.o \
./src/hardware/vga_dac.o \
./src/hardware/vga_draw.o \
./src/hardware/vga_gfx.o \
./src/hardware/vga_memory.o \
./src/hardware/vga_misc.o \
./src/hardware/vga_other.o \
./src/hardware/vga_paradise.o \
./src/hardware/vga_s3.o \
./src/hardware/vga_seq.o \
./src/hardware/vga_tseng.o \
./src/hardware/vga_xga.o 

CPP_DEPS += \
./src/hardware/adlib.d \
./src/hardware/cmos.d \
./src/hardware/dbopl.d \
./src/hardware/disney.d \
./src/hardware/dma.d \
./src/hardware/gameblaster.d \
./src/hardware/gus.d \
./src/hardware/hardware.d \
./src/hardware/iohandler.d \
./src/hardware/ipx.d \
./src/hardware/ipxserver.d \
./src/hardware/joystick.d \
./src/hardware/keyboard.d \
./src/hardware/memory.d \
./src/hardware/mixer.d \
./src/hardware/mpu401.d \
./src/hardware/opl.d \
./src/hardware/pci_bus.d \
./src/hardware/pcspeaker.d \
./src/hardware/pic.d \
./src/hardware/sblaster.d \
./src/hardware/tandy_sound.d \
./src/hardware/timer.d \
./src/hardware/vga.d \
./src/hardware/vga_attr.d \
./src/hardware/vga_crtc.d \
./src/hardware/vga_dac.d \
./src/hardware/vga_draw.d \
./src/hardware/vga_gfx.d \
./src/hardware/vga_memory.d \
./src/hardware/vga_misc.d \
./src/hardware/vga_other.d \
./src/hardware/vga_paradise.d \
./src/hardware/vga_s3.d \
./src/hardware/vga_seq.d \
./src/hardware/vga_tseng.d \
./src/hardware/vga_xga.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware/%.o: ../src/hardware/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


