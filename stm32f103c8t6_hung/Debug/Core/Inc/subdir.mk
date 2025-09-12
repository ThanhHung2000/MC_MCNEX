################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/drive_spi.c 

C_DEPS += \
./Core/Inc/drive_spi.d 

OBJS += \
./Core/Inc/drive_spi.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/%.o Core/Inc/%.su Core/Inc/%.cyclo: ../Core/Inc/%.c Core/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc

clean-Core-2f-Inc:
	-$(RM) ./Core/Inc/drive_spi.cyclo ./Core/Inc/drive_spi.d ./Core/Inc/drive_spi.o ./Core/Inc/drive_spi.su

.PHONY: clean-Core-2f-Inc

