################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ins.cpp \
../test.cpp 

OBJS += \
./ins.o \
./test.o 

CPP_DEPS += \
./ins.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/mnt/angar/doscard/trunk/SDL2/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

test.o: ../test.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/mnt/angar/doscard/trunk/SDL2/include -O0 -g3 -Wall -c -fmessage-length=0 -finstrument-functions -MMD -MP -MF"$(@:%.o=%.d)" -MT"test.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


