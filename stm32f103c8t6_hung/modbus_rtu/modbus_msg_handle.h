#ifndef _MODBUS_MSG_HANDLE_H_
#define _MODBUS_MSG_HANDLE_H_

#include "modbus_slaver_base.h"

void SlaverSerial_MessageHandle(Packet_Modbus_Slaver * msg);
void SlaverSerial_Update(Packet_Modbus_Slaver * msg);
void MsgHandle_Monitor(void);
uint8_t IsModbusConnect(void);

#endif
