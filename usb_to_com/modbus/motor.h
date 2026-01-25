/*
 * motor.h
 *
 *  Created on: Oct 27, 2025
 *      Author: MCNEX
 */
#include "stm32f4xx_hal.h"
#include "main.h"

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#define F_high	10000
#define Pulse_x_max 50000
#define Pulse_y_max 50000
#define Pulse_z_max 50000
typedef enum{
	Right,
	Left,
	Out,
	In,
	Down,
	Up,
	Unknow
} Sign;

typedef enum{
	Run_by_Hand,
	Run_Auto,
	Go_to_Home_Phase1,
	Go_to_Home_Phase2,
	Go_to_Home_Phase3,
	Stop
} MotorState;
typedef enum{
	Handler_Run,
	Handler_Stop,
	Pick_item1,
	Pick_item2
} handler_State;
typedef struct{
	Sign dir;
	MotorState state;
	uint16_t t0;
}Motor_t;
typedef struct{
	uint16_t Old_Position_x;
	uint16_t Old_Position_y;
	uint16_t Old_Position_z;
	uint16_t* Cur_x;
	uint16_t* Cur_y;
	uint16_t* Cur_z;
	uint16_t Taget_x;
	uint16_t Taget_y;
	uint16_t Taget_z;
	handler_State State;
	uint8_t Glass_scan;
}Handler_t;

extern Motor_t Motor_x;
extern Motor_t Motor_y;
extern Motor_t Motor_z;
extern Handler_t Handler;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

void Move_x(Sign dir, uint16_t Frequency);
void Move_y(Sign dir, uint16_t Frequency);
void Move_z(Sign dir, uint16_t Frequency);

void GoHome_x(uint16_t Frequency);
void GoHome_y(uint16_t Frequency);
void GoHome_z(uint16_t Frequency);

void Move_x_2_target(uint16_t target,uint16_t Frequency);
void Move_y_2_target(uint16_t target,uint16_t Frequency);
void Move_z_2_target(uint16_t target,uint16_t Frequency);
//void Move_Handle1_2_point(Item newitem);
//void Move_Handle2_2_point(Item newitem);
void Set_Frequency_Motor_x(uint16_t f, uint16_t f_max);
void Set_Frequency_Motor_y(uint16_t f, uint16_t f_max);
void Set_Frequency_Motor_z(uint16_t f, uint16_t f_max);

void Stop_x();
void Stop_y();
void Stop_z();



void Tim2_Init(void);
void Tim5_Init(void);
void Tim9_Init(void);
void Tim1_Init(void);
void Tim3_Init(void);
void Tim8_Init(void);
void Motor_x_Init(void);
void Motor_y_Init(void);
void Motor_z_Init(void);
#endif /* INC_MOTOR_H_ */
