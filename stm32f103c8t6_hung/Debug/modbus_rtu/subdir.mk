################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus_rtu/modbus_common.c \
../modbus_rtu/modbus_msg_handle.c \
../modbus_rtu/modbus_slaver_base.c \
../modbus_rtu/modbus_slaver_comp.c \
../modbus_rtu/uart_drive.c 

C_DEPS += \
./modbus_rtu/modbus_common.d \
./modbus_rtu/modbus_msg_handle.d \
./modbus_rtu/modbus_slaver_base.d \
./modbus_rtu/modbus_slaver_comp.d \
./modbus_rtu/uart_drive.d 

OBJS += \
./modbus_rtu/modbus_common.o \
./modbus_rtu/modbus_msg_handle.o \
./modbus_rtu/modbus_slaver_base.o \
./modbus_rtu/modbus_slaver_comp.o \
./modbus_rtu/uart_drive.o 


# Each subdirectory must supply rules for building sources it contributes
modbus_rtu/%.o modbus_rtu/%.su modbus_rtu/%.cyclo: ../modbus_rtu/%.c modbus_rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-modbus_rtu

clean-modbus_rtu:
	-$(RM) ./modbus_rtu/modbus_common.cyclo ./modbus_rtu/modbus_common.d ./modbus_rtu/modbus_common.o ./modbus_rtu/modbus_common.su ./modbus_rtu/modbus_msg_handle.cyclo ./modbus_rtu/modbus_msg_handle.d ./modbus_rtu/modbus_msg_handle.o ./modbus_rtu/modbus_msg_handle.su ./modbus_rtu/modbus_slaver_base.cyclo ./modbus_rtu/modbus_slaver_base.d ./modbus_rtu/modbus_slaver_base.o ./modbus_rtu/modbus_slaver_base.su ./modbus_rtu/modbus_slaver_comp.cyclo ./modbus_rtu/modbus_slaver_comp.d ./modbus_rtu/modbus_slaver_comp.o ./modbus_rtu/modbus_slaver_comp.su ./modbus_rtu/uart_drive.cyclo ./modbus_rtu/uart_drive.d ./modbus_rtu/uart_drive.o ./modbus_rtu/uart_drive.su

.PHONY: clean-modbus_rtu

