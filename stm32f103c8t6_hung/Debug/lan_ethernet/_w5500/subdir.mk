################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lan_ethernet/_w5500/dhcp.c \
../lan_ethernet/_w5500/dns.c \
../lan_ethernet/_w5500/w5500.c \
../lan_ethernet/_w5500/w5500_socket.c \
../lan_ethernet/_w5500/w5500_tcp.c \
../lan_ethernet/_w5500/w5500_udp.c \
../lan_ethernet/_w5500/w5500_util.c 

C_DEPS += \
./lan_ethernet/_w5500/dhcp.d \
./lan_ethernet/_w5500/dns.d \
./lan_ethernet/_w5500/w5500.d \
./lan_ethernet/_w5500/w5500_socket.d \
./lan_ethernet/_w5500/w5500_tcp.d \
./lan_ethernet/_w5500/w5500_udp.d \
./lan_ethernet/_w5500/w5500_util.d 

OBJS += \
./lan_ethernet/_w5500/dhcp.o \
./lan_ethernet/_w5500/dns.o \
./lan_ethernet/_w5500/w5500.o \
./lan_ethernet/_w5500/w5500_socket.o \
./lan_ethernet/_w5500/w5500_tcp.o \
./lan_ethernet/_w5500/w5500_udp.o \
./lan_ethernet/_w5500/w5500_util.o 


# Each subdirectory must supply rules for building sources it contributes
lan_ethernet/_w5500/%.o lan_ethernet/_w5500/%.su lan_ethernet/_w5500/%.cyclo: ../lan_ethernet/_w5500/%.c lan_ethernet/_w5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_GEN" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/CAN_BSW" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/lan_ethernet/queue" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/utilyty" -I../Core/Inc -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/mqtt" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/queue_out" -I"D:/PhamVanHung/STM32/stm32f103c8t6_hung/modbus_rtu" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lan_ethernet-2f-_w5500

clean-lan_ethernet-2f-_w5500:
	-$(RM) ./lan_ethernet/_w5500/dhcp.cyclo ./lan_ethernet/_w5500/dhcp.d ./lan_ethernet/_w5500/dhcp.o ./lan_ethernet/_w5500/dhcp.su ./lan_ethernet/_w5500/dns.cyclo ./lan_ethernet/_w5500/dns.d ./lan_ethernet/_w5500/dns.o ./lan_ethernet/_w5500/dns.su ./lan_ethernet/_w5500/w5500.cyclo ./lan_ethernet/_w5500/w5500.d ./lan_ethernet/_w5500/w5500.o ./lan_ethernet/_w5500/w5500.su ./lan_ethernet/_w5500/w5500_socket.cyclo ./lan_ethernet/_w5500/w5500_socket.d ./lan_ethernet/_w5500/w5500_socket.o ./lan_ethernet/_w5500/w5500_socket.su ./lan_ethernet/_w5500/w5500_tcp.cyclo ./lan_ethernet/_w5500/w5500_tcp.d ./lan_ethernet/_w5500/w5500_tcp.o ./lan_ethernet/_w5500/w5500_tcp.su ./lan_ethernet/_w5500/w5500_udp.cyclo ./lan_ethernet/_w5500/w5500_udp.d ./lan_ethernet/_w5500/w5500_udp.o ./lan_ethernet/_w5500/w5500_udp.su ./lan_ethernet/_w5500/w5500_util.cyclo ./lan_ethernet/_w5500/w5500_util.d ./lan_ethernet/_w5500/w5500_util.o ./lan_ethernet/_w5500/w5500_util.su

.PHONY: clean-lan_ethernet-2f-_w5500

