/*
 * mgr_Com.h
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */

#ifndef MGR_COM_H_
#define MGR_COM_H_
#include "main.h"
#define LED1_CAN  0x00U
#define LED2_CAN  0x01U
#define COUNT_CAN 0x02U
#define TGT_MAX_BYTES_PER_WRITE 8
typedef struct {
	uint8_t Is_Change;
	uint8_t Change_Count;
	uint8_t Time_Out;
	uint8_t Cur_Data;
	uint8_t Mid_Data;
	uint8_t Pre_Data;
	uint16_t Chatter_Time;
}tMsg_CAN_Rx_Data_s;

extern volatile uint8_t step_update;
extern  volatile tMsg_CAN_Rx_Data_s Msg_CAN_RX[COUNT_CAN];
void Com_Task(void);
void Update_FW_ComTask(void);
void reset_microcontroller(void);
#endif /* MGR_COM_H_ */
