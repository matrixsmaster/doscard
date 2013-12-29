################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../directserial.cpp \
../libserial.cpp \
../misc_util.cpp \
../nullmodem.cpp \
../serialdummy.cpp \
../serialport.cpp \
../softmodem.cpp 

OBJS += \
./directserial.o \
./libserial.o \
./misc_util.o \
./nullmodem.o \
./serialdummy.o \
./serialport.o \
./softmodem.o 

CPP_DEPS += \
./directserial.d \
./libserial.d \
./misc_util.d \
./nullmodem.d \
./serialdummy.d \
./serialport.d \
./softmodem.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


