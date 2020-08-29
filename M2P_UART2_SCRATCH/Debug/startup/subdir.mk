################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f334x8.s 

OBJS += \
./startup/startup_stm32f334x8.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/-_Alex_-/Desktop/DMA course/Workspace/M2P_UART2_SCRATCH/Utilities/STM32F3xx_Nucleo" -I"C:/Users/-_Alex_-/Desktop/DMA course/Workspace/M2P_UART2_SCRATCH/StdPeriph_Driver/inc" -I"C:/Users/-_Alex_-/Desktop/DMA course/Workspace/M2P_UART2_SCRATCH/inc" -I"C:/Users/-_Alex_-/Desktop/DMA course/Workspace/M2P_UART2_SCRATCH/CMSIS/device" -I"C:/Users/-_Alex_-/Desktop/DMA course/Workspace/M2P_UART2_SCRATCH/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


