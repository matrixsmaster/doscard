################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../midi.cpp \
../render.cpp \
../render_scalers.cpp \
../sdl_gui.cpp \
../sdl_mapper.cpp \
../sdlmain.cpp 

OBJS += \
./midi.o \
./render.o \
./render_scalers.o \
./sdl_gui.o \
./sdl_mapper.o \
./sdlmain.o 

CPP_DEPS += \
./midi.d \
./render.d \
./render_scalers.d \
./sdl_gui.d \
./sdl_mapper.d \
./sdlmain.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


