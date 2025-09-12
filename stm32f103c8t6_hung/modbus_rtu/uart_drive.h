/*
 * uart_drive.h
 *
 *  Created on: Feb 28, 2025
 *      Author: Hi
 */

#ifndef INC_UART_DRIVE_H_
#define INC_UART_DRIVE_H_
#include "main.h"
#include "modbus_slaver_base.h"
#define      START    0x00U
#define      MID_UART 0x01U
void Uart_Init(void);
void itoacode(uint16_t value, uint8_t *buffer, int radix);
void Task_uart(void);
typedef struct
{
	volatile uint8_t *dataUart;
	volatile uint8_t flag_uart;
	volatile uint8_t count;
}DataUartMsg;
typedef struct
{
	volatile uint8_t timer_end;
	volatile uint8_t uart_finish;
	volatile uint8_t complete_256;
	volatile uint8_t status_uart;//00:
	uint8_t count_lock[1];
}MsgUartProcessStatus;
typedef enum
{
  Tx_OK       = 0x00U,
  Tx_ERROR    = 0x01U,
  Tx_BUSY     = 0x02U,
  Tx_TIMEOUT  = 0x03U
} Status_Tx_Msg;
#define TX_READY 0
#define TX_BUSY 1
extern DataUartMsg uartUpdate;
extern volatile uint8_t rx_data[258];
//extern MsgUartProcessStatus uartStatus;
extern uint8_t * UartPtr[1];
void UartCheckEndOffFrame(void);
void ModbusSlaverComp_Run(void);
void PutTimeRtc(uint16_t dataw);
Status_Tx_Msg  HAL_USART_TxMsg(const uint8_t *data, uint8_t size, uint16_t timeout);
#endif /* INC_UART_DRIVE_H_ */
