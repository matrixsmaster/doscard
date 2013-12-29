################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/gui_tk/gui_tk.cpp 

OBJS += \
./libs/gui_tk/gui_tk.o 

CPP_DEPS += \
./libs/gui_tk/gui_tk.d 


# Each subdirectory must supply rules for building sources it contributes
libs/gui_tk/%.o: ../libs/gui_tk/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/doscpu" -I"/home/lisa/wdosboxtest/dosdos" -I"/home/lisa/wdosboxtest/dosgui" -I"/home/lisa/wdosboxtest/doshdw" -I"/home/lisa/wdosboxtest/dosmisc" -I"/home/lisa/wdosboxtest/libsdl/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


