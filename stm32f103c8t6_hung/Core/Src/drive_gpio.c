/*
 * drive_gpio.c
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */
#include <drive_gpio.h>
#include "mgr_com.h"
#include "il_para.h"

uint8_t L_lED_Inteerupt =FALSE;
uint8_t L_lED_Timeout =FALSE;

void ISR_Led_HU(CANObject object)
{
	L_lED_Inteerupt=TRUE;
	L_lED_Timeout =FALSE;
	object=object;// avoid warning
}

void LED_RUN (volatile tMsg_CAN_Rx_Data_s *Rx_Data1,volatile tMsg_CAN_Rx_Data_s *Rx_Data2)
{
	if(L_lED_Inteerupt==TRUE)
	{

		Rx_Data1->Cur_Data=ILGet_InforLedBle();
		if(Rx_Data1->Cur_Data!=Rx_Data1->Pre_Data)
		{
			Rx_Data1->Pre_Data=Rx_Data1->Cur_Data;
			Rx_Data1->Is_Change=TRUE;
		}
	  	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, Rx_Data1->Cur_Data);

		Rx_Data2->Cur_Data=ILGet_InforLedYellow();
		if(Rx_Data2->Cur_Data!=Rx_Data2->Pre_Data)
		{
			Rx_Data2->Pre_Data=Rx_Data2->Cur_Data;
			Rx_Data2->Is_Change=TRUE;
		}
	  	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, Rx_Data2->Cur_Data);
		L_lED_Inteerupt=FALSE;
	}
 	//Com_Led_Data();

}
