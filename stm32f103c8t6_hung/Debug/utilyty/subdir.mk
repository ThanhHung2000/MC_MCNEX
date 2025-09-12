################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilyty/buffer_cache.c \
../utilyty/global.c 

C_DEPS += \
./utilyty/buffer_cache.d \
./utilyty/global.d 

OBJS += \
./utilyty/buffer_cache.o \
./utilyty/global.o 


# Each subdirectory must supply rules for building sources it contributes
utilyty/%.o utilyty/%.su utilyty/%.cyclo: ../utilyty/%.c utilyty/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/utilyty" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-utilyty

clean-utilyty:
	-$(RM) ./utilyty/buffer_cache.cyclo ./utilyty/buffer_cache.d ./utilyty/buffer_cache.o ./utilyty/buffer_cache.su ./utilyty/global.cyclo ./utilyty/global.d ./utilyty/global.o ./utilyty/global.su

.PHONY: clean-utilyty

