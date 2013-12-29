################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cdrom.cpp \
../cdrom_aspi_win32.cpp \
../cdrom_image.cpp \
../cdrom_ioctl_linux.cpp \
../cdrom_ioctl_os2.cpp \
../cdrom_ioctl_win32.cpp \
../dos.cpp \
../dos_classes.cpp \
../dos_devices.cpp \
../dos_execute.cpp \
../dos_files.cpp \
../dos_ioctl.cpp \
../dos_keyboard_layout.cpp \
../dos_memory.cpp \
../dos_misc.cpp \
../dos_mscdex.cpp \
../dos_programs.cpp \
../dos_tables.cpp \
../drive_cache.cpp \
../drive_fat.cpp \
../drive_iso.cpp \
../drive_local.cpp \
../drive_virtual.cpp \
../drives.cpp 

OBJS += \
./cdrom.o \
./cdrom_aspi_win32.o \
./cdrom_image.o \
./cdrom_ioctl_linux.o \
./cdrom_ioctl_os2.o \
./cdrom_ioctl_win32.o \
./dos.o \
./dos_classes.o \
./dos_devices.o \
./dos_execute.o \
./dos_files.o \
./dos_ioctl.o \
./dos_keyboard_layout.o \
./dos_memory.o \
./dos_misc.o \
./dos_mscdex.o \
./dos_programs.o \
./dos_tables.o \
./drive_cache.o \
./drive_fat.o \
./drive_iso.o \
./drive_local.o \
./drive_virtual.o \
./drives.o 

CPP_DEPS += \
./cdrom.d \
./cdrom_aspi_win32.d \
./cdrom_image.d \
./cdrom_ioctl_linux.d \
./cdrom_ioctl_os2.d \
./cdrom_ioctl_win32.d \
./dos.d \
./dos_classes.d \
./dos_devices.d \
./dos_execute.d \
./dos_files.d \
./dos_ioctl.d \
./dos_keyboard_layout.d \
./dos_memory.d \
./dos_misc.d \
./dos_mscdex.d \
./dos_programs.d \
./dos_tables.d \
./drive_cache.d \
./drive_fat.d \
./drive_iso.d \
./drive_local.d \
./drive_virtual.d \
./drives.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -I"/home/lisa/wdosboxtest/libsdl/include" -I"/home/lisa/wdosboxtest/dosint" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


