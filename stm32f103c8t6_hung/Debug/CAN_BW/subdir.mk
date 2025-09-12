################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN_BW/can_drive.c \
../CAN_BW/can_par.c 

OBJS += \
./CAN_BW/can_drive.o \
./CAN_BW/can_par.o 

C_DEPS += \
./CAN_BW/can_drive.d \
./CAN_BW/can_par.d 


# Each subdirectory must supply rules for building sources it contributes
CAN_BW/%.o CAN_BW/%.su CAN_BW/%.cyclo: ../CAN_BW/%.c CAN_BW/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/Chuc/STM32CubeIDE/workspace_1.16.0/stm32f103c8t6_hung/CAN_GEN" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"C:/Users/Chuc/STM32CubeIDE/workspace_1.16.0/stm32f103c8t6_hung/CAN_BW" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CAN_BW

clean-CAN_BW:
	-$(RM) ./CAN_BW/can_drive.cyclo ./CAN_BW/can_drive.d ./CAN_BW/can_drive.o ./CAN_BW/can_drive.su ./CAN_BW/can_par.cyclo ./CAN_BW/can_par.d ./CAN_BW/can_par.o ./CAN_BW/can_par.su

.PHONY: clean-CAN_BW

