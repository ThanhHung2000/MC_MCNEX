################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN_GEN/appdesc.c \
../CAN_GEN/desc.c \
../CAN_GEN/drv_para.c \
../CAN_GEN/il_par.c 

C_DEPS += \
./CAN_GEN/appdesc.d \
./CAN_GEN/desc.d \
./CAN_GEN/drv_para.d \
./CAN_GEN/il_par.d 

OBJS += \
./CAN_GEN/appdesc.o \
./CAN_GEN/desc.o \
./CAN_GEN/drv_para.o \
./CAN_GEN/il_par.o 


# Each subdirectory must supply rules for building sources it contributes
CAN_GEN/%.o CAN_GEN/%.su CAN_GEN/%.cyclo: ../CAN_GEN/%.c CAN_GEN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CAN_GEN

clean-CAN_GEN:
	-$(RM) ./CAN_GEN/appdesc.cyclo ./CAN_GEN/appdesc.d ./CAN_GEN/appdesc.o ./CAN_GEN/appdesc.su ./CAN_GEN/desc.cyclo ./CAN_GEN/desc.d ./CAN_GEN/desc.o ./CAN_GEN/desc.su ./CAN_GEN/drv_para.cyclo ./CAN_GEN/drv_para.d ./CAN_GEN/drv_para.o ./CAN_GEN/drv_para.su ./CAN_GEN/il_par.cyclo ./CAN_GEN/il_par.d ./CAN_GEN/il_par.o ./CAN_GEN/il_par.su

.PHONY: clean-CAN_GEN

