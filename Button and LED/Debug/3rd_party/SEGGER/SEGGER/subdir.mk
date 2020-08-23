################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../3rd_party/SEGGER/SEGGER/SEGGER_RTT.c \
../3rd_party/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

OBJS += \
./3rd_party/SEGGER/SEGGER/SEGGER_RTT.o \
./3rd_party/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

C_DEPS += \
./3rd_party/SEGGER/SEGGER/SEGGER_RTT.d \
./3rd_party/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
3rd_party/SEGGER/SEGGER/%.o: ../3rd_party/SEGGER/SEGGER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/3rd_party/FreeRTOS/org/Source/include" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/config" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/3rd_party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/3rd_party/SEGGER/Config" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/3rd_party/SEGGER/OS" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/3rd_party/SEGGER/SEGGER" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/StdPeriph_Driver/inc" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/config" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/inc" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/CMSIS/device" -I"C:/Users/hp/Desktop/RTOS/RTOS_workspace/Button and LED/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


