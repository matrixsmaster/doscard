################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gui/midi.cpp \
../src/gui/render.cpp \
../src/gui/render_scalers.cpp \
../src/gui/sdl_gui.cpp \
../src/gui/sdl_mapper.cpp \
../src/gui/sdlmain.cpp 

OBJS += \
./src/gui/midi.o \
./src/gui/render.o \
./src/gui/render_scalers.o \
./src/gui/sdl_gui.o \
./src/gui/sdl_mapper.o \
./src/gui/sdlmain.o 

CPP_DEPS += \
./src/gui/midi.d \
./src/gui/render.d \
./src/gui/render_scalers.d \
./src/gui/sdl_gui.d \
./src/gui/sdl_mapper.d \
./src/gui/sdlmain.d 


# Each subdirectory must supply rules for building sources it contributes
src/gui/%.o: ../src/gui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


