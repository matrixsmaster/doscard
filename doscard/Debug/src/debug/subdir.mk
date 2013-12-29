################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/debug/debug.cpp \
../src/debug/debug_disasm.cpp \
../src/debug/debug_gui.cpp \
../src/debug/debug_win32.cpp 

OBJS += \
./src/debug/debug.o \
./src/debug/debug_disasm.o \
./src/debug/debug_gui.o \
./src/debug/debug_win32.o 

CPP_DEPS += \
./src/debug/debug.d \
./src/debug/debug_disasm.d \
./src/debug/debug_gui.d \
./src/debug/debug_win32.d 


# Each subdirectory must supply rules for building sources it contributes
src/debug/%.o: ../src/debug/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


