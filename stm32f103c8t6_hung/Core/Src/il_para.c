/*
 * il_para.c
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */


#include "il_para.h"


uint8_t ILGet_InforLedBle(void)
{
	uint8_t ret;
	ret=(uint8_t)LED_UV.LED8.LED1;
	return ret;
}
uint8_t ILGet_InforLedYellow(void)
{
	uint8_t ret;
	ret=(uint8_t)LED_UV.LED8.LED2;
	return ret;
}
void Clear_Infor_Can(void)
{
	LED_UV._c[0]=0x00U;
}
