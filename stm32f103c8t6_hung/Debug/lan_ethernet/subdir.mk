################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lan_ethernet/ethernet.c 

C_DEPS += \
./lan_ethernet/ethernet.d 

OBJS += \
./lan_ethernet/ethernet.o 


# Each subdirectory must supply rules for building sources it contributes
lan_ethernet/%.o lan_ethernet/%.su lan_ethernet/%.cyclo: ../lan_ethernet/%.c lan_ethernet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet/queue" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/utilyty" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/mqtt" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lan_ethernet

clean-lan_ethernet:
	-$(RM) ./lan_ethernet/ethernet.cyclo ./lan_ethernet/ethernet.d ./lan_ethernet/ethernet.o ./lan_ethernet/ethernet.su

.PHONY: clean-lan_ethernet

