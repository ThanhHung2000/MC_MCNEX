#ifndef _MODBUS_COMMON_H_
#define _MODBUS_COMMON_H_

#include "main.h"

#define MODBUS_MASTER 0
#define MODBUS_SLAVE 1

void ModbusCommon_Init(void);

void ModbusCommon_Run(void);
void ModbusComon_SetModeWork(uint8_t mode);

#endif
