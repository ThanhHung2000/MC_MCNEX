/*
 * motor.c
 *
 *  Created on: Oct 27, 2025
 *      Author: MCNEX
 */
#include "motor.h"

#include "modbusSlave.h"
#include <stdlib.h>

#define Handle_Offset_x 100
#define Handle_Offset_y 0


Motor_t Motor_x = {
		.dir = Left,
		.state = Stop
};
Motor_t Motor_y = {
		.dir = Left,
	.state = Stop
};
Motor_t Motor_z = {
	.dir = Left,
	.state = Stop
};
Handler_t Handler = {
	.Old_Position_x = 0,
	.Old_Position_y = 0,
	.Old_Position_z = 0,
	.Cur_x = &Holding_Registers_Database[0],
	.Cur_y = &Holding_Registers_Database[1],
	.Cur_z = &Holding_Registers_Database[2],
	.Glass_scan = 0
};

void MX_TIM2_Init(void);
void MX_TIM5_Init(void);
void MX_TIM9_Init(void);
void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM8_Init(void);
void MX_TIM4_Init(void);

void Motor_x_Init(){
	MX_TIM1_Init();
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}
void Motor_y_Init(){
	MX_TIM8_Init();
	MX_TIM5_Init();
	HAL_TIM_Base_Start_IT(&htim5);
}
void Motor_z_Init(){
	MX_TIM3_Init();
	MX_TIM9_Init();
	HAL_TIM_Base_Start_IT(&htim9);
}

void Tim2_Init(){
	MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}
void Tim5_Init(){
	MX_TIM5_Init();
	HAL_TIM_Base_Start_IT(&htim5);
}
void Tim9_Init(){
	MX_TIM9_Init();
	HAL_TIM_Base_Start_IT(&htim9);
}
void Tim1_Init(){
	MX_TIM1_Init();
}
void Tim3_Init(){
	MX_TIM3_Init();
}
void Tim8_Init(){
	MX_TIM8_Init();
}

//A8
void Move_x_2_target(uint16_t target,uint16_t Frequency){
	Handler.Taget_x = target;
	int32_t pulse = target - Handler.Old_Position_x;
	Sign dir = pulse < 0? Left:Right;
	if(abs(pulse) == 0) return;
	Motor_x.dir = dir;
//	HAL_GPIO_WritePin(GPIOC, DIR1_CH1_Pin, dir&1);
	if(Motor_x.state == Stop)
		Motor_x.state = Run_Auto;
	htim5.Instance->ARR = abs(pulse)-1;
	htim8.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
}
//C6
void Move_y_2_target(uint16_t target,uint16_t Frequency){
	Handler.Taget_y = target;
	int32_t pulse = target - Handler.Old_Position_y;
	Sign dir = pulse < 0? In:Out;
	if(abs(pulse) == 0) return;
	Motor_y.dir = dir;
//	HAL_GPIO_WritePin(GPIOA, DIR1_CH3_Pin, dir&1);
	if(Motor_y.state == Stop)
		Motor_y.state = Run_Auto;
	htim2.Instance->ARR = abs(pulse)-1;
	htim1.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
//C8
void Move_z_2_target(uint16_t target,uint16_t Frequency){
	Handler.Taget_z = target;
	int32_t pulse = target - Handler.Old_Position_z;
	Sign dir = pulse < 0? Up:Down;
	if(abs(pulse) == 0) return;
	Motor_z.dir = dir;
//	HAL_GPIO_WritePin(DIR1_CH2_GPIO_Port, DIR1_CH2_Pin, dir&1);
	if(Motor_z.state == Stop)
		Motor_z.state = Run_Auto;
	htim9.Instance->ARR = abs(pulse)-1;
	htim3.Instance->ARR = (1000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}
//void Move_Handle1_2_point(Item newitem){
//	Handler.State = Handler_Run;
//	Move_x_2_target(newitem.x,F_high);
//	Move_y_2_target(newitem.y,F_high);
//}
//void Move_Handle2_2_point(Item newitem){
//	Handler.State = Handler_Run;
//	Move_x_2_target(newitem.x - Handle_Offset_x,F_high);
//	Move_y_2_target(newitem.y - Handle_Offset_y,F_high);
//}
void GoHome_x(uint16_t Frequency){
	Motor_x.dir = Left;
//	HAL_GPIO_WritePin(DIR1_CH1_GPIO_Port, DIR1_CH1_Pin, Motor_x.dir&1);
	htim5.Instance->ARR = -1;
	htim8.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
}
void GoHome_y(uint16_t Frequency){
	Motor_y.dir = In;
//	HAL_GPIO_WritePin(DIR1_CH3_GPIO_Port, DIR1_CH3_Pin, Motor_y.dir&1);
	htim2.Instance->ARR = 65535;
	htim1.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
void GoHome_z(uint16_t Frequency){
	Motor_z.dir = Up;
//	HAL_GPIO_WritePin(DIR1_CH2_GPIO_Port, DIR1_CH2_Pin, Motor_z.dir&1);
	htim9.Instance->ARR = 65535;
	htim3.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}
void Move_x(Sign dir, uint16_t Frequency){
	Motor_x.dir = dir;
//	HAL_GPIO_WritePin(GPIOC, DIR1_CH1_Pin, dir&1);
	Motor_x.state = Run_by_Hand;
	htim5.Instance->ARR = -1;
	htim8.Instance->ARR = (2000000 / 100) -1;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
}
void Move_y(Sign dir,uint16_t Frequency){
	Motor_y.dir = dir;
//	HAL_GPIO_WritePin(DIR1_CH3_GPIO_Port, DIR1_CH3_Pin, dir&1);
	Motor_y.state = Run_by_Hand;
	htim2.Instance->ARR = 65535;
	htim1.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}
void Move_z(Sign dir,uint16_t Frequency){
	Motor_z.dir = dir;
//	HAL_GPIO_WritePin(GPIOA, DIR1_CH2_Pin, dir&1);
	Motor_z.state = Run_by_Hand;
	htim9.Instance->ARR = 65535;
	htim3.Instance->ARR = (2000000 / Frequency) -1;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}
void Set_Frequency_Motor_x(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	htim8.Instance->ARR = (2000000 / f) -1;
}
void Set_Frequency_Motor_y(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	htim1.Instance->ARR = (2000000 / f) -1;
}
void Set_Frequency_Motor_z(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	htim3.Instance->ARR = (1000000 / f) -1;
}
void Stop_x(){
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
	if(Motor_x.state == Run_by_Hand)
		*Handler.Cur_x = Motor_x.dir == Left? Handler.Old_Position_x - TIM5 -> CNT: Handler.Old_Position_x + TIM5 -> CNT;
	else if(Motor_x.state == Run_Auto){
		*Handler.Cur_x = Handler.Taget_x;
	}
	if(Motor_x.state == Go_to_Home_Phase1)
		Motor_x.state = Go_to_Home_Phase2;
	else {
		Motor_x.state = Stop;
		if(Motor_y.state == Stop) Handler.State = Handler_Stop;
	}

	Handler.Old_Position_x = *Handler.Cur_x;
	TIM5 -> CNT = 0;
}
void Stop_y(){

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	if(Motor_y.state == Run_by_Hand)
		*Handler.Cur_y = Motor_y.dir == In? Handler.Old_Position_y - TIM2 -> CNT: Handler.Old_Position_y + TIM2 -> CNT;
	else if(Motor_y.state == Run_Auto){
		*Handler.Cur_y = Handler.Taget_y;
	}
	if(Motor_y.state == Go_to_Home_Phase1)
		Motor_y.state = Go_to_Home_Phase2;
	else {
		Motor_y.state = Stop;
		if(Motor_x.state == Stop) Handler.State = Handler_Stop;
	}
	Handler.Old_Position_y = *Handler.Cur_y;
	TIM2 -> CNT = 0;
}
void Stop_z(){
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	if(Motor_z.state == Run_by_Hand)
		*Handler.Cur_z = Motor_z.dir == Up? Handler.Old_Position_z - TIM9 -> CNT: Handler.Old_Position_z + TIM9 -> CNT;
	else if(Motor_z.state == Run_Auto){
		*Handler.Cur_z = Handler.Taget_z;
	}
	if(Motor_z.state == Go_to_Home_Phase1)
		Motor_z.state = Go_to_Home_Phase2;
	else Motor_z.state = Stop;

	Handler.Old_Position_z = *Handler.Cur_z;
	htim9.Instance -> CNT = 0;
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if(htim->Instance == TIM5){
//	  Stop_y();
//  }
//  if(htim->Instance == TIM2){
//	  Stop_x();
//  }
//  if(htim->Instance == TIM9){
//	  Stop_z();
//  }
//}

void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */
	if ((TIM9->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
	{
	  if ((TIM9->DIER & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
	  {
		//__HAL_TIM_CLEAR_FLAG(&htim9, TIM_FLAG_UPDATE);
		Stop_z();
	  }
	}
  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  //HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim9);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	if ((TIM2->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
	{
	  if ((TIM2->DIER & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
	  {
		//__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
		Stop_y();
	  }
	}
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}
/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
	if ((TIM5->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
	{
	  if ((TIM5->DIER & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
	  {
		//__HAL_TIM_CLEAR_FLAG(&htim5, TIM_FLAG_UPDATE);
		Stop_x();
	  }
	}
  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}






/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}


/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 100-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC3REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 84-1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 100-1;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 20-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR3;
  if (HAL_TIM_SlaveConfigSynchro(&htim5, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 0;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 20-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR1;
  if (HAL_TIM_SlaveConfigSynchro(&htim9, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}


