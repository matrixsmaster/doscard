################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../debug.cpp \
../debug_disasm.cpp \
../debug_gui.cpp \
../debug_win32.cpp 

OBJS += \
./debug.o \
./debug_disasm.o \
./debug_gui.o \
./debug_win32.o 

CPP_DEPS += \
./debug.d \
./debug_disasm.d \
./debug_gui.d \
./debug_win32.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -I"/home/lisa/wdosboxtest/doscpu" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


