#ifndef _MODBUS_SLAVER_COMP_H_
#define _MODBUS_SLAVER_COMP_H_


void ModbusComp_checkIfEndOfMessage(void);
void ModbusSlaverComp_Init(void);
void ModbusSlaverComp_Run(void);


extern uint8 modbusCompHavedata;
#endif
