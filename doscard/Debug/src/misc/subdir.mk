################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/misc/cross.cpp \
../src/misc/messages.cpp \
../src/misc/programs.cpp \
../src/misc/setup.cpp \
../src/misc/support.cpp 

OBJS += \
./src/misc/cross.o \
./src/misc/messages.o \
./src/misc/programs.o \
./src/misc/setup.o \
./src/misc/support.o 

CPP_DEPS += \
./src/misc/cross.d \
./src/misc/messages.d \
./src/misc/programs.d \
./src/misc/setup.d \
./src/misc/support.d 


# Each subdirectory must supply rules for building sources it contributes
src/misc/%.o: ../src/misc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


