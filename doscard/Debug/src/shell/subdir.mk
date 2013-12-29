################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/shell/shell.cpp \
../src/shell/shell_batch.cpp \
../src/shell/shell_cmds.cpp \
../src/shell/shell_misc.cpp 

OBJS += \
./src/shell/shell.o \
./src/shell/shell_batch.o \
./src/shell/shell_cmds.o \
./src/shell/shell_misc.o 

CPP_DEPS += \
./src/shell/shell.d \
./src/shell/shell_batch.d \
./src/shell/shell_cmds.d \
./src/shell/shell_misc.d 


# Each subdirectory must supply rules for building sources it contributes
src/shell/%.o: ../src/shell/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


