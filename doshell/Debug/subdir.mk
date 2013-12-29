################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shell.cpp \
../shell_batch.cpp \
../shell_cmds.cpp \
../shell_misc.cpp 

OBJS += \
./shell.o \
./shell_batch.o \
./shell_cmds.o \
./shell_misc.o 

CPP_DEPS += \
./shell.d \
./shell_batch.d \
./shell_cmds.d \
./shell_misc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -I"/home/lisa/wdosboxtest/dosdos" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


