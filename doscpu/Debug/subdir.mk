################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../callback.cpp \
../core_normal.cpp \
../core_prefetch.cpp \
../core_simple.cpp \
../cpu.cpp \
../flags.cpp \
../modrm.cpp \
../paging.cpp 

OBJS += \
./callback.o \
./core_normal.o \
./core_prefetch.o \
./core_simple.o \
./cpu.o \
./flags.o \
./modrm.o \
./paging.o 

CPP_DEPS += \
./callback.d \
./core_normal.d \
./core_prefetch.d \
./core_simple.d \
./cpu.d \
./flags.d \
./modrm.d \
./paging.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


