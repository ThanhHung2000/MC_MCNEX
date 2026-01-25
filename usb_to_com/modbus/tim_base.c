/*
 * tim_base.c
 *
 *  Created on: Oct 27, 2025
 *      Author: MCNEX
 */
#include <stdlib.h>
#include <stdio.h>
#include "tim_base.h"
#include "motor.h"
#include "RS232.h"
#include "main.h"
#include "delay.h"
#include  "Common.h"
//#define time 1000 //1ms
#define Time 1000
TIM_HandleTypeDef htim10;
uint8_t Emergency = 0;
uint8_t Glass_group = 0;

void scan_HMI(void);
void Handle_Set(void);

void Handle_Left(void);
void Handle_Right(void);
void Handle_In(void);
void Handle_Out(void);
void Handle_Up(void);
void Handle_Down(void);

void Save_Glass_1(void);
void Save_Glass_2(void);
void Save_Glass_3(void);
void Save_Glass_All(void);
void Save_Cover_1(void);
void Save_Cover_2(void);
void Save_Cover_3(void);
void Save_Cover_All(void);

void Control_motor_x(void);
void Control_motor_y(void);
void Control_motor_z(void);

void Reset(void);

typedef void (*ActionHandler_t)(void);
typedef struct {
    uint16_t bitMask;
    ActionHandler_t handler;
} Map_t;

Map_t motorMoveTable[16] = {
    { 1 << 0, Handle_Left  },
    { 1 << 1, Handle_Right },
    { 1 << 2, Handle_In    },
    { 1 << 3, Handle_Out   },
    { 1 << 4, Handle_Up    },
	{ 1 << 5, Handle_Down  },
	{ 1 << 6, Handle_Set   },
	{ 1 << 7, Handle_Home  },

	{ 1 << 8, Save_Glass_1  },
	{ 1 << 9, Save_Glass_2  },
	{ 1 << 10, Save_Glass_3 },
	{ 1 << 11, Save_Cover_1  },
	{ 1 << 12, Save_Cover_2  },
	{ 1 << 13, Save_Cover_3  },
	{ 1 << 14, Save_Glass_All  },
	{ 1 << 15, Save_Cover_All  },
};

Map_t worker_controlTable[]={
	{ 1 << 3, Reset },
};
uint16_t FindActiveBit(uint8_t *data,uint8_t numbyte)
{
    for (uint8_t byteIdx = 0; byteIdx < numbyte; ++byteIdx)
    {
        uint8_t val = data[byteIdx];
        if (val) // chỉ xử lý nếu có ít nhất 1 bit được set
        {
        	int8_t fisrtbit = __builtin_ffs(val)-1;
        	return (byteIdx * 8 + fisrtbit);
        }
    }
    return 0xffff; // không có bit nào = 1
}
void scan_HMI(){
	if(Emergency!=0) return;
	if(Tab->bits.Home == 1 && Home_Lamp->bits.Run == 0){
//		if(*Glass_Select > 0){
//			*Lamp_glass_select = *Glass_Select;
//			Glass_Index = Glass_group * 196 + *Glass_Select - 1;
//			*Glass_Select = 0;
//		}
//
//		if(*Cover_Select > 0){
//			*Lamp_Cover_select = *Cover_Select;
//			end_Cover = *Cover_Select;
//			SetBit(Lamp_Cover_done, end_Cover, 25 - end_Cover);
//			*Cover_Select = 0;
//		}
//
//		if((Worker_Control ->all & 0x0f) > 0){
//			Choose_glass_group(Worker_Control->all & 0x0f);
//		}
	}
	else if (Tab->bits.Engine == 1)
	{
		uint16_t current = ((Save_Tray -> all) << 8) | (Control_motor->all);
		int8_t fisrtbit = __builtin_ffs(current)-1;
		if(fisrtbit >= 0)
		{
			motorMoveTable[fisrtbit].handler();
		}
	}
}
void Choose_glass_group(uint8_t num){
	if(num == 0) return;
//	Home_Lamp-> all &= ~(0x0fu << 2);
//	Home_Lamp-> all |= (num << 2);
//	Glass_group = __builtin_ffs(num)-1;
//	ClearBit(Lamp_glass_empty, 0, 196);
//	*Lamp_glass_select = 0;
//	for(uint16_t i = 0; i<196;i++){
//		if(Glass[Glass_group*196+i].State != OK){
//			SetBit(Lamp_glass_empty, i, 1);
//		}
//	}
}

void Reset(){

}
void Handle_Home(void){
	Handler.State = Handler_Run;
	Stop_x();
	Stop_y();
	Stop_z();
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim5);
	HAL_TIM_Base_Stop_IT(&htim9);

	Motor_x.state = Go_to_Home_Phase1;
	Motor_y.state = Go_to_Home_Phase1;
	Motor_z.state = Go_to_Home_Phase1;
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == 0){
		HAL_TIM_Base_Start_IT(&htim5);
		Move_x_2_target(1000,2000);
	}
	else GoHome_x(2000);
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == 0){
		HAL_TIM_Base_Start_IT(&htim2);
		Move_y_2_target(1000,2000);
	}
	else GoHome_y(2000);
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 0){
		HAL_TIM_Base_Start_IT(&htim9);
		Move_z_2_target(1000,200);
	}
	else GoHome_z(200);

}
void Handle_Set(void)
{
	if(Motor_x.state == Stop && Motor_y.state == Stop /*&& Motor_z.state == Stop*/){
		Move_x_2_target(*Handler.Cur_x,F_high);
		Move_y_2_target(*Handler.Cur_y,F_high);
		Move_z_2_target(*Handler.Cur_z,F_high);

		Control_motor->bits.Set = 0;
	}

}
void Handle_Left(void)
{
	if(Motor_x.state == Stop){
		Move_x(Left,100);
		Motor_x.t0 = Tick/1000;
	}
}
void Handle_Right(void)
{
	if(Motor_x.state == Stop){
		Move_x(Right,100);
		Motor_x.t0 = Tick/1000;
	}
}
void Handle_In(void)
{
	if(Motor_y.state == Stop){
		Move_y(In,100);
		Motor_y.t0 = Tick/1000;
	}
}
void Handle_Out(void)
{
	if(Motor_y.state == Stop){
		Move_y(Out,100);
		Motor_y.t0 = Tick/1000;
	}

}
void Handle_Up(void)
{
	if(Motor_z.state == Stop){
		Move_z(Up,100);
		Motor_z.t0 = Tick/1000;
	}
}
void Handle_Down(void)
{
	if(Motor_z.state == Stop){
		Move_z(Down,100);
		Motor_z.t0 = Tick/1000;
	}
}
//void Save_Glass_1(void){
//	Glass_Mark[0].x = *Handler.Cur_x;
//	Glass_Mark[0].y = *Handler.Cur_y;
//	Mark[0] = Glass_Mark[0].x;
//	Mark[1] = Glass_Mark[0].y;
//	Save_Tray->bits.glass_1 = 0;
//	Save_Tray_Indicator->bits.glass_1 = 1;
//}
//void Save_Glass_2(void){
//	Glass_Mark[1].x = *Handler.Cur_x;
//	Glass_Mark[1].y = *Handler.Cur_y;
//	Mark[2] = Glass_Mark[1].x;
//	Mark[3] = Glass_Mark[1].y;
//	Save_Tray->bits.glass_2 = 0;
//	Save_Tray_Indicator->bits.glass_2 = 1;
//}
//void Save_Glass_3(void){
//	Glass_Mark[2].x = *Handler.Cur_x;
//	Glass_Mark[2].y = *Handler.Cur_y;
//	Mark[4] = Glass_Mark[2].x;
//	Mark[5] = Glass_Mark[2].y;
//	Save_Tray->bits.glass_3 = 0;
//	Save_Tray_Indicator->bits.glass_3 = 1;
//}
//void Save_Glass_All(void){
//	Save_Glass_Mark(Save_Tray_Indicator->bits.glass_1,Save_Tray_Indicator->bits.glass_2,Save_Tray_Indicator->bits.glass_3,1);
//	Save_Tray_Indicator->bits.glass_1 = 0;
//	Save_Tray_Indicator->bits.glass_2 = 0;
//	Save_Tray_Indicator->bits.glass_3 = 0;
//	Save_Tray->bits.save_glass = 0;
//}
//void Save_Cover_1(void){
//	Cover_Mark[0].x = *Handler.Cur_x;
//	Cover_Mark[0].y = *Handler.Cur_y;
//	Mark[6] = Cover_Mark[0].x;
//	Mark[7] = Cover_Mark[0].y;
//	Save_Tray->bits.Cover_1 = 0;
//	Save_Tray_Indicator->bits.Cover_1 = 1;
//}
//void Save_Cover_2(void){
//	Cover_Mark[1].x = *Handler.Cur_x;
//	Cover_Mark[1].y = *Handler.Cur_y;
//	Mark[8] = Cover_Mark[1].x;
//	Mark[9] = Cover_Mark[2].y;
//	Save_Tray->bits.Cover_2 = 0;
//	Save_Tray_Indicator->bits.Cover_2 = 1;
//}
//void Save_Cover_3(void){
//	Cover_Mark[2].x = *Handler.Cur_x;
//	Cover_Mark[2].y = *Handler.Cur_y;
//	Mark[10] = Cover_Mark[2].x;
//	Mark[11] = Cover_Mark[2].y;
//	Save_Tray->bits.Cover_3 = 0;
//	Save_Tray_Indicator->bits.Cover_3 = 1;
//}
//void Save_Cover_All(void){
//	Save_Cover_Mark(Save_Tray_Indicator->bits.Cover_1,Save_Tray_Indicator->bits.Cover_2,Save_Tray_Indicator->bits.Cover_3,1);
//	Save_Tray_Indicator->bits.Cover_1 = 0;
//	Save_Tray_Indicator->bits.Cover_2 = 0;
//	Save_Tray_Indicator->bits.Cover_3 = 0;
//	Save_Tray->bits.Save_Cover = 0;
//}
void Control_motor_x(){
	switch(Motor_x.state){
		case Run_by_Hand:
			*Handler.Cur_x = Motor_x.dir == Left? Handler.Old_Position_x - TIM5 -> CNT: Handler.Old_Position_x + TIM5 -> CNT;
			if(*Handler.Cur_x > Pulse_x_max) {
				Emergency = 1;
				Stop_x();
			}
			Set_Frequency_Motor_x((uint16_t)(Tick/1000)-Motor_x.t0 +100 , 4000);
			if(Control_motor->bits.Left == 0 && Control_motor->bits.Right == 0)
			{
				Stop_x();
			}
			break;
		case Run_Auto:
			*Handler.Cur_x = Motor_x.dir == Left? Handler.Old_Position_x - TIM5 -> CNT: Handler.Old_Position_x + TIM5-> CNT;
			break;
		case Go_to_Home_Phase1:
			if(Control_motor->bits.Left == 0 && Control_motor->bits.Right == 0)
			{
				Set_Frequency_Motor_x(2000 , 4000);
			}
			else if(Control_motor->bits.Left == 1){
				Set_Frequency_Motor_x(10000 , 10000);
			}
			break;
		case Go_to_Home_Phase2:
			GoHome_x(500);
			break;
		case Go_to_Home_Phase3:
			*Handler.Cur_x = 0;
			Handler.Old_Position_x = 0;
			TIM5 -> CNT = 0;
			Motor_x.state = Stop;
			if(Motor_y.state == Stop && Motor_x.state == Stop) {

			}
			break;
		case Stop:

			break;
	}
}
void Control_motor_y(){
	switch(Motor_y.state){
		case Run_by_Hand:
			*Handler.Cur_y = Motor_y.dir == In? Handler.Old_Position_y - TIM2 -> CNT: Handler.Old_Position_y + TIM2 -> CNT;
			if(*Handler.Cur_y > Pulse_y_max) {
				Emergency = 1;
				Stop_y();
			}
			Set_Frequency_Motor_y((uint16_t)(Tick/1000)-Motor_y.t0 +100 , 4000);
			if(Control_motor->bits.In == 0 && Control_motor->bits.Out == 0)
				Stop_y();
			break;
		case Run_Auto:
			*Handler.Cur_y = Motor_y.dir == In? Handler.Old_Position_y - TIM2 -> CNT: Handler.Old_Position_y + TIM2 -> CNT;
			break;
		case Go_to_Home_Phase1:
			if(Control_motor->bits.In == 0 && Control_motor->bits.Out == 0)
			{
				Set_Frequency_Motor_y(2000 , 4000);
			}
			else if(Control_motor->bits.In == 1){
				Set_Frequency_Motor_y(10000 , 10000);
			}
			break;
		case Go_to_Home_Phase2:
			GoHome_y(500);
			break;
		case Go_to_Home_Phase3:
			*Handler.Cur_y = 0;
			Handler.Old_Position_y = 0;
			TIM2 -> CNT = 0;
			Motor_y.state = Stop;
			if(Motor_x.state == Stop && Motor_z.state == Stop) {

			}
			break;
		case Stop:

			break;
	}
}
void Control_motor_z(){
	switch(Motor_z.state){
		case Run_by_Hand:
			*Handler.Cur_z = Motor_z.dir == Up? Handler.Old_Position_z - TIM9 -> CNT : Handler.Old_Position_z + TIM9 -> CNT;
			if(*Handler.Cur_z > Pulse_z_max) {
				Emergency = 1;
				Stop_z();
			}
			Set_Frequency_Motor_z((uint16_t)(Tick/1000)-Motor_z.t0 +100 , 4000);
			if(Control_motor->bits.Up == 0 && Control_motor->bits.Down == 0)
				Stop_z();
			break;
		case Run_Auto:
			*Handler.Cur_z = Motor_z.dir == Up? Handler.Old_Position_z - TIM9 -> CNT : Handler.Old_Position_z + TIM9 -> CNT;
			break;
		case Go_to_Home_Phase1:
			if(Control_motor->bits.Up == 0)
			{
				Set_Frequency_Motor_z(2000 , 4000);
			}
			else if(Control_motor->bits.Up == 1){
				Set_Frequency_Motor_z(10000 , 10000);
			}
			break;
		case Go_to_Home_Phase2:
			GoHome_z(500);
			break;
		case Go_to_Home_Phase3:
			*Handler.Cur_z = 0;
			Handler.Old_Position_z = 0;
			TIM9 -> CNT = 0;
			Motor_z.state = Stop;
			if(Motor_x.state == Stop && Motor_y.state == Stop) {

			}
			break;
		case Stop:

			break;
	}
}


void Set_Time(uint16_t time){
	htim7.Instance->ARR = time -1;
}
void TIM7_IRQHandler11(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */
  scan_HMI();
//  Refresh_WDT();
  /* USER CODE END TIM7_IRQn 1 */
}
void TIM1_UP_TIM10_IRQHandler11(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim10);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */
	Control_motor_x();
	Control_motor_y();
	Control_motor_z();
	//Refresh_WDT();
  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}



