################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../adlib.cpp \
../cmos.cpp \
../dbopl.cpp \
../disney.cpp \
../dma.cpp \
../gameblaster.cpp \
../gus.cpp \
../hardware.cpp \
../iohandler.cpp \
../ipx.cpp \
../ipxserver.cpp \
../joystick.cpp \
../keyboard.cpp \
../memory.cpp \
../mixer.cpp \
../mpu401.cpp \
../opl.cpp \
../pci_bus.cpp \
../pcspeaker.cpp \
../pic.cpp \
../sblaster.cpp \
../tandy_sound.cpp \
../timer.cpp \
../vga.cpp \
../vga_attr.cpp \
../vga_crtc.cpp \
../vga_dac.cpp \
../vga_draw.cpp \
../vga_gfx.cpp \
../vga_memory.cpp \
../vga_misc.cpp \
../vga_other.cpp \
../vga_paradise.cpp \
../vga_s3.cpp \
../vga_seq.cpp \
../vga_tseng.cpp \
../vga_xga.cpp 

OBJS += \
./adlib.o \
./cmos.o \
./dbopl.o \
./disney.o \
./dma.o \
./gameblaster.o \
./gus.o \
./hardware.o \
./iohandler.o \
./ipx.o \
./ipxserver.o \
./joystick.o \
./keyboard.o \
./memory.o \
./mixer.o \
./mpu401.o \
./opl.o \
./pci_bus.o \
./pcspeaker.o \
./pic.o \
./sblaster.o \
./tandy_sound.o \
./timer.o \
./vga.o \
./vga_attr.o \
./vga_crtc.o \
./vga_dac.o \
./vga_draw.o \
./vga_gfx.o \
./vga_memory.o \
./vga_misc.o \
./vga_other.o \
./vga_paradise.o \
./vga_s3.o \
./vga_seq.o \
./vga_tseng.o \
./vga_xga.o 

CPP_DEPS += \
./adlib.d \
./cmos.d \
./dbopl.d \
./disney.d \
./dma.d \
./gameblaster.d \
./gus.d \
./hardware.d \
./iohandler.d \
./ipx.d \
./ipxserver.d \
./joystick.d \
./keyboard.d \
./memory.d \
./mixer.d \
./mpu401.d \
./opl.d \
./pci_bus.d \
./pcspeaker.d \
./pic.d \
./sblaster.d \
./tandy_sound.d \
./timer.d \
./vga.d \
./vga_attr.d \
./vga_crtc.d \
./vga_dac.d \
./vga_draw.d \
./vga_gfx.d \
./vga_memory.d \
./vga_misc.d \
./vga_other.d \
./vga_paradise.d \
./vga_s3.d \
./vga_seq.d \
./vga_tseng.d \
./vga_xga.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -I"/home/lisa/wdosboxtest/dosgui" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


