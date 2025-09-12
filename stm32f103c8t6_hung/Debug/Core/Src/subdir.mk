################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/can_par.c \
../Core/Src/dhcp.c \
../Core/Src/drive_gpio.c \
../Core/Src/drive_time.c \
../Core/Src/i2c_drive.c \
../Core/Src/il_para.c \
../Core/Src/main.c \
../Core/Src/mgr_com.c \
../Core/Src/socket.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tpmc.c \
../Core/Src/version.c \
../Core/Src/w5500.c \
../Core/Src/wizchip_conf.c 

C_DEPS += \
./Core/Src/can_par.d \
./Core/Src/dhcp.d \
./Core/Src/drive_gpio.d \
./Core/Src/drive_time.d \
./Core/Src/i2c_drive.d \
./Core/Src/il_para.d \
./Core/Src/main.d \
./Core/Src/mgr_com.d \
./Core/Src/socket.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tpmc.d \
./Core/Src/version.d \
./Core/Src/w5500.d \
./Core/Src/wizchip_conf.d 

OBJS += \
./Core/Src/can_par.o \
./Core/Src/dhcp.o \
./Core/Src/drive_gpio.o \
./Core/Src/drive_time.o \
./Core/Src/i2c_drive.o \
./Core/Src/il_para.o \
./Core/Src/main.o \
./Core/Src/mgr_com.o \
./Core/Src/socket.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tpmc.o \
./Core/Src/version.o \
./Core/Src/w5500.o \
./Core/Src/wizchip_conf.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/can_par.cyclo ./Core/Src/can_par.d ./Core/Src/can_par.o ./Core/Src/can_par.su ./Core/Src/dhcp.cyclo ./Core/Src/dhcp.d ./Core/Src/dhcp.o ./Core/Src/dhcp.su ./Core/Src/drive_gpio.cyclo ./Core/Src/drive_gpio.d ./Core/Src/drive_gpio.o ./Core/Src/drive_gpio.su ./Core/Src/drive_time.cyclo ./Core/Src/drive_time.d ./Core/Src/drive_time.o ./Core/Src/drive_time.su ./Core/Src/i2c_drive.cyclo ./Core/Src/i2c_drive.d ./Core/Src/i2c_drive.o ./Core/Src/i2c_drive.su ./Core/Src/il_para.cyclo ./Core/Src/il_para.d ./Core/Src/il_para.o ./Core/Src/il_para.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mgr_com.cyclo ./Core/Src/mgr_com.d ./Core/Src/mgr_com.o ./Core/Src/mgr_com.su ./Core/Src/socket.cyclo ./Core/Src/socket.d ./Core/Src/socket.o ./Core/Src/socket.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tpmc.cyclo ./Core/Src/tpmc.d ./Core/Src/tpmc.o ./Core/Src/tpmc.su ./Core/Src/version.cyclo ./Core/Src/version.d ./Core/Src/version.o ./Core/Src/version.su ./Core/Src/w5500.cyclo ./Core/Src/w5500.d ./Core/Src/w5500.o ./Core/Src/w5500.su ./Core/Src/wizchip_conf.cyclo ./Core/Src/wizchip_conf.d ./Core/Src/wizchip_conf.o ./Core/Src/wizchip_conf.su

.PHONY: clean-Core-2f-Src

