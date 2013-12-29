################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../bios.cpp \
../bios_disk.cpp \
../bios_keyboard.cpp \
../ems.cpp \
../int10.cpp \
../int10_char.cpp \
../int10_memory.cpp \
../int10_misc.cpp \
../int10_modes.cpp \
../int10_pal.cpp \
../int10_put_pixel.cpp \
../int10_vesa.cpp \
../int10_video_state.cpp \
../int10_vptable.cpp \
../mouse.cpp \
../xms.cpp 

OBJS += \
./bios.o \
./bios_disk.o \
./bios_keyboard.o \
./ems.o \
./int10.o \
./int10_char.o \
./int10_memory.o \
./int10_misc.o \
./int10_modes.o \
./int10_pal.o \
./int10_put_pixel.o \
./int10_vesa.o \
./int10_video_state.o \
./int10_vptable.o \
./mouse.o \
./xms.o 

CPP_DEPS += \
./bios.d \
./bios_disk.d \
./bios_keyboard.d \
./ems.d \
./int10.d \
./int10_char.d \
./int10_memory.d \
./int10_misc.d \
./int10_modes.d \
./int10_pal.d \
./int10_put_pixel.d \
./int10_vesa.d \
./int10_video_state.d \
./int10_vptable.d \
./mouse.d \
./xms.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -I"/home/lisa/wdosboxtest/dosdos" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


