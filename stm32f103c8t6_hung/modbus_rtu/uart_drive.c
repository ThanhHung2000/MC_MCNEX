/*
 * uart_drive.c
 *
 *  Created on: Feb 28, 2025
 *      Author: Hi
 */
#include<uart_drive.h>
#include"i2c_drive.h"
#include"drive_time.h"
#include <can_drive.h>
#include "main.h"
#include "mgr_com.h"
#include "modbus_slaver_comp.h"
#include "modbus_common.h"

#include "modbus_slaver_define.h"
#include "modbus_slaver_base.h"
extern MsgCanTranmitTypeDef *CanReceiveUart;
extern MsgCanTranmitTypeDef dataCanReceivefromUART;
extern volatile uint32_t cout_CAN_ir;
extern MsgCanTranmitTypeDef dataCanReceivefromCAN;
extern FifoCanReceiveTypeDef dataFifoReceivefromCAN;
uint8_t  tx_data[20]="STM32 Hello Hung!!\r\n";
volatile uint8_t rx_data[258]={};
extern uint8_t download_type;


DataUartMsg uartUpdate;
MsgUartProcessStatus uartStatus;
volatile uint8_t rx_data_interrupt;
volatile uint16_t cout_receive=0;
uint8_t data[6]={};
volatile uint8_t crc_check[2];
extern DS1307_STRUCT data_rtc;
extern MsgCanTranmitTypeDef DataCanMsg;
static uint8_t status_Tx=TX_READY;
uint16_t CRC16_CCITT_FALSE(volatile uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF; // Giá trị khởi tạo
    uint16_t polynomial = 0x1021; // Đa thức CRC-16 CCITT

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= (data[i] << 8);
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ polynomial;
            else
                crc <<= 1;
        }
    }
    return crc;
}
void USART3_IRQHandler(void)
{
	if((USART3->SR& USART_SR_RXNE)== USART_SR_RXNE)/*wait RXNE==1: Received data is ready to be read.*/
	{
        rx_data_interrupt=(uint8_t)USART3->DR;/*read data in register RDR so clear bit RXNE (RXNE=0)*/;
        if(cout_receive==0x00U)
        {
        	uartStatus.status_uart=MID_UART;
        }
        if(cout_receive<253)
        {
			rx_data[cout_receive]=rx_data_interrupt;
			++cout_receive;
        }
        uartStatus.timer_end=0x00U;
        if(cout_receive>=253)
        {
        	uartStatus.complete_256=0x01U;
        	uartStatus.status_uart=START;
        }
	}
	if((USART3->SR&(uint32_t)(1<<3))!=0)/*ORE==1*/
    {
		/*Overrun error is detected*/
    }
}

void Uart_Init(void)
{
	  //HAL_UART_Transmit(&huart3,tx_data, sizeof(tx_data), 100);
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;    // Enable GPIOB
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable USART3
	// PB10 - TX
	GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
	GPIOB->CRH |=  (GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0);  // Output 50MHz
	GPIOB->CRH |=  GPIO_CRH_CNF10_1; // AF Push-Pull

	// PB11 - RX
	GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
	GPIOB->CRH |=  GPIO_CRH_CNF11_0; // Input floating

	USART3->BRR = 0x0138;  // 115200bps

	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;    // Enable TX, RX
	USART3->CR1 |= USART_CR1_RXNEIE;               // Enable RX interrupt
	USART3->CR1 |= USART_CR1_UE;                   // Enable USART

	NVIC_EnableIRQ(USART3_IRQn);

}
Status_Tx_Msg UART_WaitOnFlagUntilTimeout(uint32_t Flag,FlagStatus status,uint32_t tickstart,uint32_t Timeout)
{
    // Check flag
    while ((((USART3->SR & Flag)==Flag)?SET:RESET)==status)
    {
        // Check timeout
            if ((Timeout == 0U) || ((millis() - tickstart) > Timeout))
            {
                return Tx_TIMEOUT;
            }
    }

    return Tx_OK;
}

Status_Tx_Msg  HAL_USART_TxMsg(const uint8_t *data, uint8_t size, uint16_t timeout)
{
	uint32_t milisec_tick=0;
	if(status_Tx==TX_READY)//TX_READY
	{
		if(data==NULL||(size==0u))
		{
			return Tx_ERROR;
		}
		status_Tx=TX_BUSY;//TX_BUSY
		milisec_tick=millis();//get milisec;
		while(size>0)
		{
			if(UART_WaitOnFlagUntilTimeout(USART_SR_TXE,RESET,milisec_tick,timeout)!=Tx_OK)
			{
				status_Tx=TX_BUSY;
				return Tx_TIMEOUT;
			}
			{
			 USART3->DR =(uint8_t)(*data);
			 data++;
			}
			size--;
		}
		if(UART_WaitOnFlagUntilTimeout(USART_SR_TC,RESET,milisec_tick,timeout)!=Tx_OK)
		{
			status_Tx=TX_BUSY;
			return Tx_TIMEOUT;
		}
		status_Tx=0;
		return Tx_OK;
		//End_of_transition
//	    USART1->ICR |=USART_ICR_TCCF;/*Writing 1 to this bit clears the TC flag in the USART_ISR register.*/
	}
	return Tx_ERROR;
}
// Hàm chuyển đổi từ uint16_t sang chuỗi ký tự
void itoacode(uint16_t value, uint8_t *buffer, int radix) {
    int i = 0;
    int isNegative = 0;
    int is10 = 0;

    // Xử lý trường hợp giá trị âm (chỉ áp dụng cho radix = 10)
    if (value < 0 && radix == 10) {
        isNegative = 1;
        value = -value;
    }
    if (value < 10 ) {
    	is10 = 1;//nho hon 10
    }
    // Chuyển đổi giá trị thành chuỗi ngược
    do {
        int digit = value % radix;
        buffer[i++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
        value /= radix;
    } while (value > 0);

    // Nếu là giá trị âm và radix là 10, thêm dấu âm
    if (isNegative && radix == 10) {
        buffer[i++] = '-';
    }
    if(is10==1)
    {
    	buffer[i++]='0';
    }
    // Đảo chuỗi để có giá trị đúng
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    // Kết thúc chuỗi
    buffer[i] = '\0';
}
void Task_uart(void)
{
	uint16_t crc=0;
	MsgCanTranmitTypeDef DataCan;
	uint8_t count[6];
	static uint32_t count_uart=0x01U;
	if(uartStatus.complete_256==0x01u)
	{
		uartStatus.complete_256=0x00U;
		packet_comp.len_buf_recv=cout_receive;
		modbusCompHavedata = 1U;
		if(cout_receive<253&&step_update==0x03U)
		{
			for(uint8_t i=cout_receive;i<255;i++)
			{
				rx_data[i]=0xffU;
			}
		}
		CanReceiveUart=(MsgCanTranmitTypeDef*)rx_data;
		uartUpdate.dataUart = rx_data;
		uartUpdate.flag_uart=0x01U;
		uartUpdate.count=cout_receive;
		dataCanReceivefromUART=*CanReceiveUart;
		crc=CRC16_CCITT_FALSE(rx_data,cout_receive);
		crc_check[1]=(uint8_t)crc;
		crc_check[0]=(uint8_t)(crc>>8);
		cout_receive=0x00U;

        //CANTxMsgMessage((const uint8_t*)&dataCanReceivefromUART);

	}
	if(GetDataCanReceive(&DataCan)==0x01U)
	{
		if(DataCan.DataIdCan.id==0x79E)
		{
			Mode_Complete(&DataCan);
			DataCanMsg=DataCan;
		}
		else //if(DataCan.DataIdCan.id==0x796)
		{
			count[0]=(uint8_t)(cout_CAN_ir>>16);
			count[1]=(uint8_t)(cout_CAN_ir>>8);
			count[2]=(uint8_t)cout_CAN_ir;
			count[3]=(uint8_t)(count_uart>>16);
			count[4]=(uint8_t)(count_uart>>8);
			count[5]=(uint8_t)count_uart;
            /*
		    	HAL_UART_Transmit(&huart3,(const uint8_t*)&DataCan,DataCan.LEN+6,100);//count_uart
			*/
			//HAL_UART_Transmit(&huart3,(const uint8_t*)count,6,100);//count
			//HAL_USART_TxMsg((const uint8_t*)count,6,100);
			count_uart++;
		}

	}
}
void PutTimeRtc(uint16_t dataw)
{

	uint16_t timemsec=0x00U;
	uint8_t c='-';
	uint8_t line[]="\n\r";
	timemsec=*(MobusDataPtr[HOLDING_register] + ADR_REG_HOLDING_UPDATE_REALTIME +3);
	timemsec >>=8;
	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.sec,data,10);


	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);

	HAL_USART_TxMsg(&c, 1, 100);

	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.min,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);

	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.hour,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);
	c=':';
	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.dow,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);
	c='/';
	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.month,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);

	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.date,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);
	c='-';
	reset_buffer(data,sizeof(data));
	itoacode(data_rtc.year,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(&c, 1, 100);

	reset_buffer(data,sizeof(data));
	itoacode(dataw,data,10);
	HAL_USART_TxMsg((const uint8_t*)data,sizeof(data),100);
	HAL_USART_TxMsg(line,sizeof(line), 100);
}
//void USART3_IRQHandler(void)
uint8_t * UartPtr[1]={
		uartStatus.count_lock,
};
void UartCheckEndOffFrame(void)
{
	if(uartStatus.status_uart==MID_UART)
	{
		if(++uartStatus.timer_end>=2)
		{
			uartStatus.timer_end=0x00U;
			uartStatus.complete_256=0x01U;
			if(rx_data[0]==0x05&&rx_data[1]==0x31&&rx_data[2]==0x01
			  &&rx_data[3]==0xff&&rx_data[4]==0x00&&rx_data[5]==0x01)
			{
				update_status=StartUpDate;
				step_update=0x01U;//DOWNLOAD_BEGIN
				download_type=rx_data[2];
				uartStatus.count_lock[0]=0x00U;
			}
			else if(rx_data[0]==0x01&&rx_data[2]==0x55&&rx_data[1]==0x37)
			{
				step_update=0x04;//DOWNLOAD_BEGIN
			}
			else if(rx_data[0]==0x75&&rx_data[1]==0x70&&rx_data[2]==0x64&&rx_data[3]==0x74)
			{
			   Delay_SetTimer(TID_TIMER_UPDT,100);
			}
			uartStatus.status_uart=START;
		}
	}
}




