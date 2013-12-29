################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ints/bios.cpp \
../src/ints/bios_disk.cpp \
../src/ints/bios_keyboard.cpp \
../src/ints/ems.cpp \
../src/ints/int10.cpp \
../src/ints/int10_char.cpp \
../src/ints/int10_memory.cpp \
../src/ints/int10_misc.cpp \
../src/ints/int10_modes.cpp \
../src/ints/int10_pal.cpp \
../src/ints/int10_put_pixel.cpp \
../src/ints/int10_vesa.cpp \
../src/ints/int10_video_state.cpp \
../src/ints/int10_vptable.cpp \
../src/ints/mouse.cpp \
../src/ints/xms.cpp 

OBJS += \
./src/ints/bios.o \
./src/ints/bios_disk.o \
./src/ints/bios_keyboard.o \
./src/ints/ems.o \
./src/ints/int10.o \
./src/ints/int10_char.o \
./src/ints/int10_memory.o \
./src/ints/int10_misc.o \
./src/ints/int10_modes.o \
./src/ints/int10_pal.o \
./src/ints/int10_put_pixel.o \
./src/ints/int10_vesa.o \
./src/ints/int10_video_state.o \
./src/ints/int10_vptable.o \
./src/ints/mouse.o \
./src/ints/xms.o 

CPP_DEPS += \
./src/ints/bios.d \
./src/ints/bios_disk.d \
./src/ints/bios_keyboard.d \
./src/ints/ems.d \
./src/ints/int10.d \
./src/ints/int10_char.d \
./src/ints/int10_memory.d \
./src/ints/int10_misc.d \
./src/ints/int10_modes.d \
./src/ints/int10_pal.d \
./src/ints/int10_put_pixel.d \
./src/ints/int10_vesa.d \
./src/ints/int10_video_state.d \
./src/ints/int10_vptable.d \
./src/ints/mouse.d \
./src/ints/xms.d 


# Each subdirectory must supply rules for building sources it contributes
src/ints/%.o: ../src/ints/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


