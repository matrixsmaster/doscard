################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hardware/serialport/directserial.cpp \
../src/hardware/serialport/libserial.cpp \
../src/hardware/serialport/misc_util.cpp \
../src/hardware/serialport/nullmodem.cpp \
../src/hardware/serialport/serialdummy.cpp \
../src/hardware/serialport/serialport.cpp \
../src/hardware/serialport/softmodem.cpp 

OBJS += \
./src/hardware/serialport/directserial.o \
./src/hardware/serialport/libserial.o \
./src/hardware/serialport/misc_util.o \
./src/hardware/serialport/nullmodem.o \
./src/hardware/serialport/serialdummy.o \
./src/hardware/serialport/serialport.o \
./src/hardware/serialport/softmodem.o 

CPP_DEPS += \
./src/hardware/serialport/directserial.d \
./src/hardware/serialport/libserial.d \
./src/hardware/serialport/misc_util.d \
./src/hardware/serialport/nullmodem.d \
./src/hardware/serialport/serialdummy.d \
./src/hardware/serialport/serialport.d \
./src/hardware/serialport/softmodem.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware/serialport/%.o: ../src/hardware/serialport/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


