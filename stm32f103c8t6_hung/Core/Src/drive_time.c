/*
 * drive_time.c
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */
#include"drive_time.h"
#include<uart_drive.h>
#include<can_drive.h>
#include "mgr_com.h"
#include "desc.h"
#include "modbus_common.h"
Msg_Timer_Delay TID_Timer[TOTAL_TIMER_DELAY];
static volatile uint32_t _tGloabal_milis=0;
static volatile uint8_t timetick_100ms=0;
static volatile uint8_t giay60=0;

volatile uint8_t update_status=0x00U;

extern uint8_t count_lock;
extern uint32_t lastModbusTime;
uint32_t timedelayall[TOTAL_TIMER]={0,};
void Timer2_Init(void)
{
	// Enable Clock for Timer2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Reset Timer
	TIM2->CR1 = 0;
	TIM2->PSC = 71;       // Prescaler (72MHz / (71 + 1) = 1MHz tick)
	TIM2->ARR = 999;      // Auto reload value (1ms)

	// Enable Update Interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	// Enable Timer
	TIM2->CR1 |= TIM_CR1_CEN;

	// Enable NVIC for TIM2 IRQ
	NVIC_EnableIRQ(TIM2_IRQn);

}
void Timer3_Init(void)
{
	// Bật clock cho Timer3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Reset Timer3 cấu hình lại
	TIM3->CR1 = 0;
	TIM3->PSC = 71;      // Prescaler: (72MHz / 72 = 1MHz)
	TIM3->ARR = 999;     // Auto Reload: 1ms

	// Bật ngắt khi tràn
	TIM3->DIER |= TIM_DIER_UIE;

	// Bật Timer
	TIM3->CR1 |= TIM_CR1_CEN;

	// Bật ngắt trong NVIC
	NVIC_EnableIRQ(TIM3_IRQn);

}
void TIM3_IRQHandler(void)
{
	static uint8_t timer=0x00U;
	static uint8_t timer_mobus=0x00U;
    if (TIM3->SR & TIM_SR_UIF)  // Kiểm tra xem có ngắt xảy ra không
    {
    	timer++;
    	if(timer>=0x01U)
    	{
    		Task_uart();
    		timer=0x00U;
    	}
    	timer_mobus++;
    	if(timer_mobus>=10U)
    	{
    		timer_mobus=0x00U;
    		//ModbusCommon_Run();
    	}
		TIM3->SR &= ~TIM_SR_UIF; // Xóa cờ UIF (bit 0)
    }
}
uint32_t millis(void)
{
	return _tGloabal_milis;
}
uint8_t IsTickChange(void)//100ms
{
	if(timetick_100ms==0x01U)
	{
		timetick_100ms=0x00U;
		return 1;
	}
	return 0x00U;
}
uint8_t IsSecondChange()
{
	if(giay60==0x01U)
	{
		giay60=0x00U;
		return 1;
	}
	return 0x00U;
}
void TIM2_IRQHandler(void)
{
	static volatile uint8_t vcan_cout=0x00U;
	static uint8_t time100ms=0x00;
	static uint8_t giay=0x00U;
    if (TIM2->SR & TIM_SR_UIF)  // Kiểm tra xem có ngắt xảy ra không
    {
      	++_tGloabal_milis;
      	UartCheckEndOffFrame();
      	if(++vcan_cout>=0x0AU)
      	{
      		DescStateTask();
      		vcan_cout=0x00U;
      	}
      	if(++time100ms>=100U)
      	{
      		time100ms=0X00u;
      		timetick_100ms=0x01U;
      		if(++giay>=10)
      		{
      			giay=0x00U;
      			giay60=0x01U;

      		}
      	}
		if(_tGloabal_milis>=0x7FFFFFFFU)
		{
			_tGloabal_milis=0x00U;
			for(int i=0;i<TOTAL_TIMER_DELAY;i++)
			{
				TID_Timer[i].Time_Cur=0x00U;
				if(TID_Timer[i].End_Time>0x7FFFFFFFU)
				{
					TID_Timer[i].End_Time=TID_Timer[i].End_Time-0x7FFFFFFFU;
				}
				else
				{
					TID_Timer[i].End_Time=0x00U;
				}

			}
			if(lastModbusTime>0x7FFFFFFFU)
			{
				lastModbusTime=lastModbusTime-0x7FFFFFFFU;
			}
			else
			{
				lastModbusTime=0x00U;
			}

		}
		TIM2->SR &= ~TIM_SR_UIF; // Xóa cờ UIF (bit 0)
    }

}
void reset_timer(void)
{

	for(int i=0;i<TOTAL_TIMER_DELAY;i++)
	{
		TID_Timer[i].active=0x00U;
		TID_Timer[i].Time_Delay=0x00U;
		TID_Timer[i].Time_Cur=0x00U;
		TID_Timer[i].End_Time=0x00U;
		_tGloabal_milis=0x00U;
	}

}
void reset_timer_one_channel(uint8_t id)
{
	TID_Timer[id].active=0x00U;
	TID_Timer[id].Time_Delay=0x00U;
	TID_Timer[id].Time_Cur=0x00U;
	TID_Timer[id].End_Time=0x00U;
}
void Delay_SetTimer(uint8_t id,uint32_t timer)
{
	TID_Timer[id].active=0x01U;
	TID_Timer[id].Time_Delay=timer;
	TID_Timer[id].Time_Cur=_tGloabal_milis;
	TID_Timer[id].End_Time=TID_Timer[id].Time_Cur+TID_Timer[id].Time_Delay;
}
uint8_t Delay_GetTimer(uint8_t id)
{
	TID_Timer[id].Time_Cur=_tGloabal_milis;
	if(TID_Timer[id].active==0x01U)
	{
		if(TID_Timer[id].Time_Cur>=TID_Timer[id].End_Time)
		{
			Delay_SetTimer(id,TID_Timer[id].Time_Delay);
			return 0x01U;
		}
	}
	return 0x00U;
}
uint8_t check_timedelay(uint8_t id, uint32_t timedelay)
{
    if(timedelayall[id]==0x00U)
    {
      timedelayall[id]=millis()+timedelay;//milise();
    }
    else if(millis()>=timedelayall[id])
    {
    	timedelayall[id]=0x00U;
    	return 0x01U;
    }
    return 0x00U;
}

