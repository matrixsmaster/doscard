################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dos/cdrom.cpp \
../src/dos/cdrom_aspi_win32.cpp \
../src/dos/cdrom_image.cpp \
../src/dos/cdrom_ioctl_linux.cpp \
../src/dos/cdrom_ioctl_os2.cpp \
../src/dos/cdrom_ioctl_win32.cpp \
../src/dos/dos.cpp \
../src/dos/dos_classes.cpp \
../src/dos/dos_devices.cpp \
../src/dos/dos_execute.cpp \
../src/dos/dos_files.cpp \
../src/dos/dos_ioctl.cpp \
../src/dos/dos_keyboard_layout.cpp \
../src/dos/dos_memory.cpp \
../src/dos/dos_misc.cpp \
../src/dos/dos_mscdex.cpp \
../src/dos/dos_programs.cpp \
../src/dos/dos_tables.cpp \
../src/dos/drive_cache.cpp \
../src/dos/drive_fat.cpp \
../src/dos/drive_iso.cpp \
../src/dos/drive_local.cpp \
../src/dos/drive_virtual.cpp \
../src/dos/drives.cpp 

OBJS += \
./src/dos/cdrom.o \
./src/dos/cdrom_aspi_win32.o \
./src/dos/cdrom_image.o \
./src/dos/cdrom_ioctl_linux.o \
./src/dos/cdrom_ioctl_os2.o \
./src/dos/cdrom_ioctl_win32.o \
./src/dos/dos.o \
./src/dos/dos_classes.o \
./src/dos/dos_devices.o \
./src/dos/dos_execute.o \
./src/dos/dos_files.o \
./src/dos/dos_ioctl.o \
./src/dos/dos_keyboard_layout.o \
./src/dos/dos_memory.o \
./src/dos/dos_misc.o \
./src/dos/dos_mscdex.o \
./src/dos/dos_programs.o \
./src/dos/dos_tables.o \
./src/dos/drive_cache.o \
./src/dos/drive_fat.o \
./src/dos/drive_iso.o \
./src/dos/drive_local.o \
./src/dos/drive_virtual.o \
./src/dos/drives.o 

CPP_DEPS += \
./src/dos/cdrom.d \
./src/dos/cdrom_aspi_win32.d \
./src/dos/cdrom_image.d \
./src/dos/cdrom_ioctl_linux.d \
./src/dos/cdrom_ioctl_os2.d \
./src/dos/cdrom_ioctl_win32.d \
./src/dos/dos.d \
./src/dos/dos_classes.d \
./src/dos/dos_devices.d \
./src/dos/dos_execute.d \
./src/dos/dos_files.d \
./src/dos/dos_ioctl.d \
./src/dos/dos_keyboard_layout.d \
./src/dos/dos_memory.d \
./src/dos/dos_misc.d \
./src/dos/dos_mscdex.d \
./src/dos/dos_programs.d \
./src/dos/dos_tables.d \
./src/dos/drive_cache.d \
./src/dos/drive_fat.d \
./src/dos/drive_iso.d \
./src/dos/drive_local.d \
./src/dos/drive_virtual.d \
./src/dos/drives.d 


# Each subdirectory must supply rules for building sources it contributes
src/dos/%.o: ../src/dos/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lisa/wdosboxtest/doscard/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


