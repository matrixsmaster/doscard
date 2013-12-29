################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: doscard

# Tool invocations
doscard: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/lisa/wdosboxtest/libsdl/build/.libs" -L"/home/lisa/wdosboxtest/dosdebug/Debug" -L"/home/lisa/wdosboxtest/doscpu/Debug" -L"/home/lisa/wdosboxtest/dosfpu/Debug" -L"/home/lisa/wdosboxtest/dosint/Debug" -L"/home/lisa/wdosboxtest/doshdw/Debug" -L"/home/lisa/wdosboxtest/dosserport/Debug" -L"/home/lisa/wdosboxtest/dosgui/Debug" -L"/home/lisa/wdosboxtest/dosdos/Debug" -L"/home/lisa/wdosboxtest/doshell/Debug" -L"/home/lisa/wdosboxtest/dosmisc/Debug" -o "doscard" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) doscard
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
