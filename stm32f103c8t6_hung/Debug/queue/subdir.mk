################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../queue/network.c \
../queue/remote.c 

C_DEPS += \
./queue/network.d \
./queue/remote.d 

OBJS += \
./queue/network.o \
./queue/remote.o 


# Each subdirectory must supply rules for building sources it contributes
queue/%.o queue/%.su queue/%.cyclo: ../queue/%.c queue/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/queue" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/utilyty" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/mqtt" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/lan_ethernet" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/lan_ethernet/_w5500" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/workspace_1.16.0/stm32f103c8t6_hung/CAN_BSW" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-queue

clean-queue:
	-$(RM) ./queue/network.cyclo ./queue/network.d ./queue/network.o ./queue/network.su ./queue/remote.cyclo ./queue/remote.d ./queue/remote.o ./queue/remote.su

.PHONY: clean-queue

