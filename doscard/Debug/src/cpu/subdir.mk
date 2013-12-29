################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cpu/callback.cpp \
../src/cpu/core_dyn_x86.cpp \
../src/cpu/core_dynrec.cpp \
../src/cpu/core_full.cpp \
../src/cpu/core_normal.cpp \
../src/cpu/core_prefetch.cpp \
../src/cpu/core_simple.cpp \
../src/cpu/cpu.cpp \
../src/cpu/flags.cpp \
../src/cpu/modrm.cpp \
../src/cpu/paging.cpp 

OBJS += \
./src/cpu/callback.o \
./src/cpu/core_dyn_x86.o \
./src/cpu/core_dynrec.o \
./src/cpu/core_full.o \
./src/cpu/core_normal.o \
./src/cpu/core_prefetch.o \
./src/cpu/core_simple.o \
./src/cpu/cpu.o \
./src/cpu/flags.o \
./src/cpu/modrm.o \
./src/cpu/paging.o 

CPP_DEPS += \
./src/cpu/callback.d \
./src/cpu/core_dyn_x86.d \
./src/cpu/core_dynrec.d \
./src/cpu/core_full.d \
./src/cpu/core_normal.d \
./src/cpu/core_prefetch.d \
./src/cpu/core_simple.d \
./src/cpu/cpu.d \
./src/cpu/flags.d \
./src/cpu/modrm.d \
./src/cpu/paging.d 


# Each subdirectory must supply rules for building sources it contributes
src/cpu/%.o: ../src/cpu/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


