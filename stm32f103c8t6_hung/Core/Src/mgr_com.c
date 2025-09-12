/*
 * mgr_com.c
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */
#include <drive_gpio.h>
#include "mgr_com.h"
#include"drive_time.h"
#include<uart_drive.h>
#define DOWNLOAD_STANDAND 0x00U
#define DOWNLOAD_BEGIN 0x01U
#define DOWNLOAD_UNLOCK 0x02U
#define DOWNLOAD_RUN 0x03U
#define DOWNLOAD_STOP 0x04U

volatile tMsg_CAN_Rx_Data_s Msg_CAN_RX[COUNT_CAN];
extern volatile uint8_t CAN_Rx_Complete_Flag;
extern volatile uint8_t crc_check[2];
extern volatile uint8_t Transfer_Mode;
extern volatile uint8_t crc_check[2];

uint8_t download_type=0x00U;
uint8_t LED_STATUS_UPDOWNLOAD=0x00U;
volatile uint8_t step_update=DOWNLOAD_STANDAND;



uint32_t Pages_Written=0x00U;
MsgCanTranmitTypeDef DataCanMsg;

 //volatile tMsg_LED_Funtion tLED_Msg;
// mgr_com chua cac ham giao tiep de lay du lieu va duoc goi tu application

void Com_Task(void)
{

		LED_RUN(&Msg_CAN_RX[LED1_CAN],&Msg_CAN_RX[LED2_CAN]);
		if(Delay_GetTimer(TID_TIMER_UPDT))
		{
		  reset_microcontroller();
		}

}
void Update_FW_ComTask(void)
{
	static uint8_t unlock_key=0x00U;
	uint16_t index1=0x00U;
	static uint16_t last_addr_cnt=0x00U;
	uint8_t On_Time=0x00U;
	uint8_t bufer_data[10];
	uint16_t send_cnt=0x00U,remain_256=0x00U,delay_cnt=0x00U;

	switch(step_update)
	{
		case DOWNLOAD_BEGIN:
		{
			//download_type = 0x01;//Info[1];
			SEND_7DF(STANDARD_MODE);
			HAL_Delay(500);
			CAN_Rx_Complete_Flag = 0x00U;
			while((CAN_Rx_Complete_Flag == 0x00))
			{
				SEND_796(SWVERSION);
				Transfer_Mode = SWVERSION;
				HAL_Delay(200);
				if(step_update==DOWNLOAD_STOP) break;
			}
			LED_STATUS_UPDOWNLOAD = 0x00U;

			SEND_541(0x03U);// 541 IGN_ON

			SEND_7DF(STANDARD_MODE);
			HAL_Delay(500);
			SEND_541(0x03U);// 541 IGN_ON
			SEND_7DF(EXTENDED_MODE);
			HAL_Delay(500);
			CAN_Rx_Complete_Flag = 0x00U;
			while((CAN_Rx_Complete_Flag == 0x00))
			{
				SEND_541(0x03U);// 541 IGN_ON
				SEND_796(EXTENDED_SESSION);
				Transfer_Mode = EXTENDED_SESSION;
				HAL_Delay(100);
				if(step_update==DOWNLOAD_STOP) break;
			}
			LED_STATUS_UPDOWNLOAD = 0x01U;
			SEND_541(0x03U);// 541 IGN_ON

			SEND_7DF(CONTROLDTC_OFF);
			HAL_Delay(100);

			SEND_541(0x03U);// 541 IGN_ON

			SEND_7DF(COMM_CONTROL_DISABLE);
			HAL_Delay(500);
			CAN_Rx_Complete_Flag = 0x00U;

			SEND_541(0x03U);// 541 IGN_ON
			SEND_796(PROGRAMMING_SESSION);
			Transfer_Mode = PROGRAMMING_SESSION;
			HAL_Delay(100);
			//}
			LED_STATUS_UPDOWNLOAD = 0x00U;
			CAN_Rx_Complete_Flag = 0x00U;

			SEND_541(0x03U);// 541 IGN_ON
			SEND_796(REQUEST_SEED);
			Transfer_Mode = REQUEST_SEED;
			HAL_Delay(100);

			LED_STATUS_UPDOWNLOAD = 0x01U;
			CAN_Rx_Complete_Flag = 0x00U;
			SEND_541(0x03U);// 541 IGN_ON
			SEND_796(SEND_KEY);
			Transfer_Mode = SEND_KEY;
			HAL_Delay(100);
			if(step_update==0x04) break;
			step_update=DOWNLOAD_UNLOCK;
		}
		break;
		case DOWNLOAD_UNLOCK:
		{
			On_Time=Delay_GetTimer(TID_COUNT_10ms);
			if(On_Time==1)
			{
				switch(*(UartPtr[0]))
				{
					case 0:
					{
							SEND_541(0x03U);// 541 IGN_ON
							SEND_796(READ_PARTNUMBER);
							HAL_Delay(100);
							bufer_data[0]=DataCanMsg.DATA[5];
							bufer_data[1]=DataCanMsg.DATA[6];
							bufer_data[2]=DataCanMsg.DATA[7];
							//sent flow control
							HAL_Delay(20);
							SEND_796(CONTROL_FLOW);
							HAL_Delay(20);
							bufer_data[3]=DataCanMsg.DATA[1];
							bufer_data[4]=DataCanMsg.DATA[2];
							bufer_data[5]=DataCanMsg.DATA[3];
							bufer_data[6]=DataCanMsg.DATA[4];
							bufer_data[7]=DataCanMsg.DATA[5];
							bufer_data[8]=DataCanMsg.DATA[6];
							bufer_data[9]=DataCanMsg.DATA[7];
							HAL_Delay(10);
							//So sanh "MCNEX WRITE" và data nhan duoc vua doc
							if ( (bufer_data[0] == 'M') && (bufer_data[1] == 'C') && (bufer_data[2] == 'N') &&
								(bufer_data[3] == 'E') && (bufer_data[4] == 'X') && (bufer_data[5] == 'W') &&
								(bufer_data[6] == 'R') && (bufer_data[7] == 'I') && (bufer_data[8] == 'T') &&
								(bufer_data[9] == 'E') ) //DescGetStateSecurityAccess() == kDescStateSecurityAccessUnlockedL1) //&& (DescGetStateSession() == kDescStateSessionProgramming)
							{
								unlock_key=0x00U;// da mo khoa
							}
							else
							{
								unlock_key=0x00U;// da khoa
							}
							HAL_Delay(20);
							SEND_541(0x03U);// 541 IGN_ON
							if(unlock_key==0X01u)
							{
								//uartStatus.count_lock=1;//UNLOCK_WRITE_DATA_PARTNUMBER;
								*(UartPtr[0])=0x01U;
							}
							else if(unlock_key==0x00U)
							{
								//uartStatus.count_lock=1;//UNLOCK_WRITE_DATA_PARTNUMBER;
								*(UartPtr[0])=0x01U;
							}
					}
					break;
					case 1://UNLOCK_WRITE_DATA_PARTNUMBER:
					{
						HAL_Delay(20);
						SEND_541(0x03U);// 541 IGN_ON
						SEND_796(FIRT_FARME);
						HAL_Delay(30);
						SEND_541(0x03U);// 541 IGN_ON
						SEND_796(CONSECUTIVE);
						//uartStatus.count_lock=2;//STEP_DEFAUL;
						*(UartPtr[0])=0x02U;
					}
					break;
					case 2://STEP_DEFAUL;
					{
							LED_STATUS_UPDOWNLOAD = 0x00U;
							CAN_Rx_Complete_Flag = 0x00U;
							while(CAN_Rx_Complete_Flag == 0x00)
							{
								SEND_541(0x03U);// 541 IGN_ON
								SEND_796(ROUTINE_BINARY_UPDATE);
								Transfer_Mode = ROUTINE_BINARY_UPDATE;
								HAL_Delay(100);
								HAL_Delay(100);
								if(step_update==DOWNLOAD_STOP) break;

							}
							LED_STATUS_UPDOWNLOAD = 0x01U;

							HAL_Delay(2000);
							LED_STATUS_UPDOWNLOAD = 0x00U;


							SEND_541(0x03U);// 541 IGN_ON
							SEND_7DF(COMM_CONTROL_DISABLE2);
							HAL_Delay(100);

							CAN_Rx_Complete_Flag = 0x00U;

							while(CAN_Rx_Complete_Flag == 0x00)
							{
								SEND_541(0x03U);// 541 IGN_ON
								SEND_796(REQUEST_DOWNLOAD_1);
								Transfer_Mode = REQUEST_DOWNLOAD_1;
								HAL_Delay(100);
								if(step_update==DOWNLOAD_STOP) break;
							}
							LED_STATUS_UPDOWNLOAD = 0x01U;

							HAL_Delay(1);
							CAN_Rx_Complete_Flag = 0x00U;


							while(CAN_Rx_Complete_Flag == 0x00)
							{
								SEND_541(0x03U);// 541 IGN_ON
								SEND_796(REQUEST_DOWNLOAD_2);
								Transfer_Mode = REQUEST_DOWNLOAD_2;
								HAL_Delay(100);
								if(step_update==DOWNLOAD_STOP) break;
							}
							HAL_Delay(1);
							LED_STATUS_UPDOWNLOAD = 0x00U;

						//Response = TGT_Get_Info (TGT_Info);
						HAL_Delay(2);
						index1 = 0x00U;
						step_update=DOWNLOAD_RUN;
						Pages_Written=0x00U;
						last_addr_cnt=0x14BU;
						uartUpdate.flag_uart=0x00U;
						//uartStatus.count_lock=0x00U;
						*(UartPtr[0])=0x00U;
						//HAL_UART_Transmit(&huart3,(const uint8_t *)crc_check,2, 100);
						HAL_USART_TxMsg((const uint8_t *)crc_check,2, 100);
					}
					break;
				};
			 }
			}
			break;
			case DOWNLOAD_RUN:
			{
				if(uartUpdate.flag_uart==0x01U)
				{
					Transfer_Mode = REQUEST_DOWNLOAD_2;

					//Flash_Read( address,Page_Buf,(U16)253 );
					index1 = 0x00U;
					TGT_Write_Flash((uartUpdate.dataUart), index1, 4,(uint8_t)Pages_Written);

					while(CAN_Rx_Complete_Flag == 0x00)
					{
						HAL_Delay(50);
						if(step_update==DOWNLOAD_STOP) break;
					}
					HAL_Delay(10); //(2);
					SEND_7DF(COMM_CONTROL_DISABLE2);
					for (index1 = 4; index1 < 256; index1 += 7)
					{
						send_cnt++;
						if ( send_cnt%20 == 0x00U)
						{
							SEND_7DF(COMM_CONTROL_DISABLE2);
						}

						if ( index1 >= 246 )
						{
							while(CAN_Rx_Complete_Flag == 0x00)
							{
								TGT_Write_Flash ((uartUpdate.dataUart), index1, 4,Pages_Written);
								HAL_Delay(50);//200
								if(step_update==DOWNLOAD_STOP) break;
							}
							remain_256 += 253;
							if ( remain_256 >= 256)
							{
								remain_256 -= 256;
								delay_cnt++;
								if ( delay_cnt % 16 == 1)
								{
									delay_cnt %= 16;
									HAL_Delay(50);
								}
							}
						}
						else
						{
							TGT_Write_Flash ((uartUpdate.dataUart), index1, TGT_MAX_BYTES_PER_WRITE,Pages_Written);
							if ( send_cnt == 8 )
							{

	//							Timeout = 0x01U;
								while(CAN_Rx_Complete_Flag == 0x00)
								{
									HAL_Delay(10);
									if(step_update==DOWNLOAD_STOP) break;
								}
								HAL_Delay(3);//(5);
							}
						}

						HAL_Delay(3); //5
					}
					On_Time = check_timedelay(1,100);//100ms
					if ( On_Time == TRUE)
					{
						if (LED_STATUS_UPDOWNLOAD == 0x00U)
						{
							LED_STATUS_UPDOWNLOAD = 0x01U;
						}
						else
						{
							LED_STATUS_UPDOWNLOAD = 0x00U;
						}
					}
					send_cnt = 0;
					Pages_Written++;
					if (Pages_Written >= last_addr_cnt )
					{
						step_update=DOWNLOAD_STOP;
						break;
					}
					uartUpdate.flag_uart=0x00U;
					//HAL_UART_Transmit(&huart3,(const uint8_t *)crc_check,2, 100);
					HAL_USART_TxMsg((const uint8_t *)crc_check,2, 100);
				}
			}
			break;
			case DOWNLOAD_STOP:
			{
				SEND_796(REQUEST_TRANSFER_EXIT);
				HAL_Delay(300);//(5);
				SEND_7DF(COMM_CONTROL_ENABLE);
				HAL_Delay(200);//(5);
				SEND_796(ECU_RESET);
				HAL_Delay(200);//(5);
				step_update=0x99u;
				//HAL_UART_Transmit(&huart3,(const uint8_t *)crc_check,2, 100);
				HAL_USART_TxMsg((const uint8_t *)crc_check,2, 100);
			}
			break;
			default:
				step_update=0X00U;
				update_status=0X00U;
				//uartStatus.count_lock=0x00U;
				*(UartPtr[0])=0x00U;
			break;
		}
}


void reset_microcontroller(void)
{
	reset_timer_one_channel(TID_TIMER_UPDT);
	HAL_FLASH_Unlock();
	// 2. Xóa page trước khi ghi (nếu cần)
	// 3. Ghi dữ liệu (4 byte)
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,0x0800ffd0,0xddccbbaaU);//Flash_Read(0x0800ffd0)==0xddccbbaaU)
	// 4. Khóa Flash lại
	HAL_FLASH_Lock();

    uint32_t *AIRCR = (uint32_t*) 0xE000ED0C;  // Địa chỉ của AIRCR
    // Ghi vào thanh ghi AIRCR để yêu cầu reset hệ thống
    *AIRCR = (0x5FA << 16) | (1 << 2);  // Magic value 0x5FA và SYSRESETREQ
}

