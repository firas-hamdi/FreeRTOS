################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
-include 3rd_party/SEGGER/SEGGER/subdir.mk
-include 3rd_party/SEGGER/OS/subdir.mk
-include 3rd_party/SEGGER/Config/subdir.mk
-include 3rd_party/FreeRTOS/org/Source/portable/MemMang/subdir.mk
-include 3rd_party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/subdir.mk
-include 3rd_party/FreeRTOS/org/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: semapore_mutex.elf

# Tool invocations
semapore_mutex.elf: $(OBJS) $(USER_OBJS) C:\Users\hp\Desktop\RTOS\RTOS_workspace\semapore_mutex/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T"C:/Users/hp/Desktop/RTOS/RTOS_workspace/semapore_mutex/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "semapore_mutex.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "semapore_mutex.elf" "semapore_mutex.bin"
	arm-none-eabi-size "semapore_mutex.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
