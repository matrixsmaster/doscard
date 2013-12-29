################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/libs/gui_tk/gui_tk.cpp 

OBJS += \
./src/libs/gui_tk/gui_tk.o 

CPP_DEPS += \
./src/libs/gui_tk/gui_tk.d 


# Each subdirectory must supply rules for building sources it contributes
src/libs/gui_tk/%.o: ../src/libs/gui_tk/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


