################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mqtt/mqtt_client.c \
../mqtt/mqtt_connect.c \
../mqtt/mqtt_funtion.c \
../mqtt/mqtt_publish.c \
../mqtt/mqtt_subscrice.c \
../mqtt/mqtt_unsubscrice.c 

C_DEPS += \
./mqtt/mqtt_client.d \
./mqtt/mqtt_connect.d \
./mqtt/mqtt_funtion.d \
./mqtt/mqtt_publish.d \
./mqtt/mqtt_subscrice.d \
./mqtt/mqtt_unsubscrice.d 

OBJS += \
./mqtt/mqtt_client.o \
./mqtt/mqtt_connect.o \
./mqtt/mqtt_funtion.o \
./mqtt/mqtt_publish.o \
./mqtt/mqtt_subscrice.o \
./mqtt/mqtt_unsubscrice.o 


# Each subdirectory must supply rules for building sources it contributes
mqtt/%.o mqtt/%.su mqtt/%.cyclo: ../mqtt/%.c mqtt/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/utilyty" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/mqtt" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-mqtt

clean-mqtt:
	-$(RM) ./mqtt/mqtt_client.cyclo ./mqtt/mqtt_client.d ./mqtt/mqtt_client.o ./mqtt/mqtt_client.su ./mqtt/mqtt_connect.cyclo ./mqtt/mqtt_connect.d ./mqtt/mqtt_connect.o ./mqtt/mqtt_connect.su ./mqtt/mqtt_funtion.cyclo ./mqtt/mqtt_funtion.d ./mqtt/mqtt_funtion.o ./mqtt/mqtt_funtion.su ./mqtt/mqtt_publish.cyclo ./mqtt/mqtt_publish.d ./mqtt/mqtt_publish.o ./mqtt/mqtt_publish.su ./mqtt/mqtt_subscrice.cyclo ./mqtt/mqtt_subscrice.d ./mqtt/mqtt_subscrice.o ./mqtt/mqtt_subscrice.su ./mqtt/mqtt_unsubscrice.cyclo ./mqtt/mqtt_unsubscrice.d ./mqtt/mqtt_unsubscrice.o ./mqtt/mqtt_unsubscrice.su

.PHONY: clean-mqtt

