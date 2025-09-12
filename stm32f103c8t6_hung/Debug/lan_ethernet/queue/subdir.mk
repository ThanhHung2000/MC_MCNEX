################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lan_ethernet/queue/network.c \
../lan_ethernet/queue/remote.c \
../lan_ethernet/queue/remote_generator.c 

C_DEPS += \
./lan_ethernet/queue/network.d \
./lan_ethernet/queue/remote.d \
./lan_ethernet/queue/remote_generator.d 

OBJS += \
./lan_ethernet/queue/network.o \
./lan_ethernet/queue/remote.o \
./lan_ethernet/queue/remote_generator.o 


# Each subdirectory must supply rules for building sources it contributes
lan_ethernet/queue/%.o lan_ethernet/queue/%.su lan_ethernet/queue/%.cyclo: ../lan_ethernet/queue/%.c lan_ethernet/queue/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet/queue" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/utilyty" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/mqtt" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lan_ethernet-2f-queue

clean-lan_ethernet-2f-queue:
	-$(RM) ./lan_ethernet/queue/network.cyclo ./lan_ethernet/queue/network.d ./lan_ethernet/queue/network.o ./lan_ethernet/queue/network.su ./lan_ethernet/queue/remote.cyclo ./lan_ethernet/queue/remote.d ./lan_ethernet/queue/remote.o ./lan_ethernet/queue/remote.su ./lan_ethernet/queue/remote_generator.cyclo ./lan_ethernet/queue/remote_generator.d ./lan_ethernet/queue/remote_generator.o ./lan_ethernet/queue/remote_generator.su

.PHONY: clean-lan_ethernet-2f-queue

